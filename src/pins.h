#ifndef PINS_H
#define PINS_H

// Pin definitions for all scenarios

// MCP1 (16 channels) - Relays for valves
const int pump = 0;         // Relay 0 - Pump 220V
const int ledCiterne = 1;   // Relay 1 - Tank LED 5V
const int sterput = 2;      // Relay 2 - Valve 1: House septic tank
const int citerne = 3;      // Relay 3 - Valve 2: Tank
const int tImper = 4;       // Relay 4 - Valve 3: Left roof rainwater
const int tPlant = 5;       // Relay 5 - Valve 4: Right roof rainwater
const int egout = 6;        // Relay 6 - Valve 5: Sewer mushroom
const int bOrage = 7;       // Relay 7 - Valve 6: Storm basin
const int ledBO2 = 8;       // Relay 8 - Lower storm basin LED 5V
const int ledBO1 = 9;       // Relay 9 - Upper storm basin LED 5V
const int pChamp = 10;      // Relay 10 - Valve 7: Field rainwater
const int pJardin = 11;     // Relay 11 - Valve 8: Garden rainwater
const int collecteur = 12;  // Relay 12 - Valve 9: House collector (egg)
const int ruePlace = 13;    // Relay 13 - Valve 10: Street and square
const int riviere = 14;     // Relay 14 - Valve 11: River
const int simVille = 15;    // Relay 15 - Valve 12: Neighbor's house in egg

// MCP0 (16 channels) - LEDs and other components
const int StripPin = 5;     // LED strip (via MOSFET) 12V
const int nappe = 6;        // Groundwater table 12V
const int egg = 7;          // Egg collector 12V
const int cave = 8;         // Home 6
const int toilette = 9;     // Home 2
const int sdb1 = 10;        // Home 1
const int arrosage = 11;    // Home 4
const int cuisine = 12;     // Home 3
const int lavelinge = 13;   // Home 5

// Note: These pin definitions correspond to specific relays, valves, and LEDs in the project.
// They are used to control various zones and actions in different scenarios.
// Proper documentation of these mappings is crucial for understanding and maintaining the system.

#endif // PINS_H