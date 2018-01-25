#ifndef	config_H
#define	config_H
//
// Serial Config
// ----------------------------
#define BAUD 115200
//
// Device Credentials
// ----------------------------
#define HOSTNAME "fridge"
#define HOSTPASS "fridge"
//
// Blynk Config
// ----------------------------
#define USE_NODE_MCU_BOARD
//
// Button Pin
// ----------------------------
#define BUTTONPIN D0
//
// Thermometers
// ----------------------------
#define ONE_WIRE_BUS D4
#define TEMPERATURE_PRECISION 9
//
// Relay Pin
// ----------------------------
#define RELAYPIN D3
//
// LCD Pins
// ----------------------------
#define COLOR 0xFFFF
#define RST D2
#define CE D1
#define DC D6
#define DIN D7
#define CLK D5
#define BL D8

#endif
