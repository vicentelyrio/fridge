#include <Config.h>
#include <Wifi.h>

bool shouldSaveConfig = false;
char blynk_token[34] = "";

WiFiManager wifiManager;

static void saveConfigCallback() {
  shouldSaveConfig = true;
}

static void setConfigModeCallback(WiFiManager *wifiManager) {
  display.println("Config mode\n\nName: " + wifiManager->getConfigPortalSSID() +  "\nIP: " + WiFi.softAPIP(), 1, 0, 0);

  Serial.println(WiFi.softAPIP());
  Serial.println(wifiManager->getConfigPortalSSID());
}

Wifi::Wifi() {}

IPAddress Wifi::getIP() {
  return WiFi.localIP();
}

char* Wifi::getBlynkToken() {
  return blynk_token;
}

void Wifi::resetConfig() {
  SPIFFS.format();
  wifiManager.resetSettings();
  display.println("Data cleared", 1, 0, 16);
  delay(1000);
  display.println("Restarting", 1, 0, 20);
  delay(500);
  ESP.reset();
}

void Wifi::getConfig() {
  if(SPIFFS.begin()) {
    if(SPIFFS.exists("/config.json")) {
      File configFile = SPIFFS.open("/config.json", "r");

      if(configFile) {
        size_t size = configFile.size();
        std::unique_ptr<char[]> buf(new char[size]);
        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        // json.printTo(Serial);

        if(json.success()) {
          strcpy(blynk_token, json["blynk_token"]);
        }
        else {
          Serial.println("failed to load json config");
          display.println("Failed to load\nWifi config", 1, 0, 0);
        }
      }
    }
  }
  else {
    Serial.println("File System mount failed");
    display.println("Failed to mount\nFile System", 1, 0, 0);
  }
}

void Wifi::saveConfig(char* blynk_token) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json["blynk_token"] = blynk_token;

  File configFile = SPIFFS.open("/config.json", "w");
  if(!configFile) {
    Serial.println("failed to open config file for writing");
    display.println("Failed to save", 1, 0, 0);
  }

  // json.printTo(Serial);
  json.printTo(configFile);
  display.println("Wifi config\nsaved", 1, 0, 0);
  configFile.close();
}

void Wifi::setup() {
  getConfig();

  WiFiManagerParameter custom_blynk_token("blynk", "blynk token", blynk_token, 36);

  wifiManager.setAPCallback(setConfigModeCallback);
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  wifiManager.addParameter(&custom_blynk_token);
  wifiManager.setTimeout(120);

  while(!wifiManager.autoConnect(HOSTNAME, HOSTPASS)) {
    Serial.println("failed to connect and hit timeout");
    display.println("Failed to connect\nto Wifi", 1, 0, 0);
    delay(3000);
    ESP.reset();
  }

  strcpy(blynk_token, custom_blynk_token.getValue());

  if(shouldSaveConfig) {
    saveConfig(blynk_token);
  }
}

Wifi wifi = Wifi();
