# Water Cycle Demonstration Project

## Project Overview

This project is a controller for demonstrating the water cycle at the Brussels Sewer Museum (Musée des Égouts de Bruxelles). It simulates various scenarios related to water management and the urban water cycle using Arduino, LEDs, valves, and audio playback.

## Goals

1. Educate visitors about the urban water cycle and its management.
2. Demonstrate different scenarios related to water usage and management in an interactive way.
3. Provide an engaging, multisensory experience combining visual (LEDs) and audio elements.

## Features

1. **Multiple Scenarios**: The project demonstrates five different water-related scenarios:
   - Rain and groundwater recharge (S1)
   - Drinking water usage and sewage (S2)
   - Wastewater treatment and river discharge (S3)
   - Storm and overflow management (S4)
   - Rainwater harvesting and reuse (S5)

2. **Interactive Button Interface**: Visitors can select different scenarios using buttons connected to analog input A3.

3. **Visual Representation**: Uses LEDs to represent various elements of the water cycle (e.g., groundwater, sewer systems, storm basins).

4. **Valve Control**: Simulates water flow using multiple valves for different parts of the system (e.g., rain, sewers, river).

5. **Audio Playback**: Incorporates sound effects for a more immersive experience (e.g., rain, thunder, water usage sounds) using a DFPlayer Mini module.

6. **Multitasking**: Utilizes FreeRTOS for efficient task management and parallel operation of different system components.

7. **Waiting Mode**: Features a mode with a "Knight Rider" style LED effect when the system is idle.

## Hardware Components

- Arduino Nano (ATmega328P)
- MCP23X17 I/O Expanders (for additional digital I/O)
- DFRobot DFPlayer Mini (for audio playback)
- LEDs (for visual representation)
- Valves (for simulating water flow)
- Buttons (for scenario selection)

## Software Libraries

- Arduino FreeRTOS (for multitasking)
- Adafruit MCP23X17 (for I/O expansion)
- DFRobotDFPlayerMini (for audio control)
- SoftwareSerial (for communication with DFPlayer)

## File Structure

```
src/
├── config.h               # Configuration file with pin definitions and debug mode
├── main.cpp               # Main Arduino sketch with FreeRTOS tasks
├── MP3Player.h            # Header file for MP3Player class
├── MP3Player.cpp          # Implementation of MP3Player class
├── Scenario.h             # Base class for all scenarios
├── Scenario1Pluie.h       # Header for Scenario 1: Rain
├── Scenario1Pluie.cpp     # Implementation of Scenario 1: Rain
├── Scenario2EauPotable.h  # Header for Scenario 2: Potable Water
├── Scenario2EauPotable.cpp # Implementation of Scenario 2: Potable Water
├── Scenario3Epuration.h   # Header for Scenario 3: Water Treatment
├── Scenario3Epuration.cpp # Implementation of Scenario 3: Water Treatment
├── Scenario4Orage.h       # Header for Scenario 4: Thunderstorm
├── Scenario4Orage.cpp     # Implementation of Scenario 4: Thunderstorm
├── Scenario5RecupPluie.h  # Header for Scenario 5: Rainwater Recovery
├── Scenario5RecupPluie.cpp # Implementation of Scenario 5: Rainwater Recovery
└── README.md              # This file
```

## File Descriptions

- `config.h`: Contains global configuration settings, pin definitions, and debug mode toggle.
- `main.cpp`: The main Arduino sketch. It sets up the hardware, creates FreeRTOS tasks, and manages the overall flow of the program.
- `MP3Player.h` and `MP3Player.cpp`: Define and implement the MP3Player class, which controls the DFPlayer Mini module.
- `Scenario.h`: Defines the base Scenario class that all specific scenarios inherit from.
- `Scenario1Pluie.h` and `Scenario1Pluie.cpp`: Implement the rain scenario.
- `Scenario2EauPotable.h` and `Scenario2EauPotable.cpp`: Implement the potable water usage scenario.
- `Scenario3Epuration.h` and `Scenario3Epuration.cpp`: Implement the water treatment scenario.
- `Scenario4Orage.h` and `Scenario4Orage.cpp`: Implement the thunderstorm scenario.
- `Scenario5RecupPluie.h` and `Scenario5RecupPluie.cpp`: Implement the rainwater recovery scenario.

## Scenarios

1. **Rain (S1)**: Simulates rain, its effect on fields and gardens, and groundwater recharge.
2. **Drinking Water (S2)**: Demonstrates drinking water extraction, household use, and sewage flow.
3. **Wastewater Treatment (S3)**: Shows the journey of wastewater from households to treatment plants and rivers.
4. **Storm (S4)**: Illustrates heavy rain, street flooding, and the use of storm basins.
5. **Rainwater Harvesting (S5)**: Demonstrates rainwater collection, storage, and use for irrigation and household purposes.

## Usage

1. Upload the code to your Arduino board.
2. Ensure all components (MCP23017 I/O expanders, DFPlayer Mini, LEDs, valves, etc.) are correctly connected.
3. Power on the system.
4. The demo (Knight Rider style LED effect) will run by default.
5. Press the corresponding button to activate a specific scenario.

## Debugging

To enable debug mode, set `DEBUG_MODE` to `true` in `config.h`. This will output debug information to the Serial Monitor.

## Future Improvements

- Add more detailed comments in the code for easier maintenance.
- Implement error handling and recovery mechanisms.
- Consider adding a display for providing textual information to visitors.
- Explore the possibility of adding more interactive elements or sensors.

This project provides an educational and engaging way to learn about the urban water cycle and water management strategies.

## Note

Make sure to have all the required libraries installed in your Arduino IDE or PlatformIO environment before compiling and uploading the code.