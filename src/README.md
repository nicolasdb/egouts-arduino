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

## Scenarios

1. **Rain (S1)**: Simulates rain, its effect on fields and gardens, and groundwater recharge.
2. **Drinking Water (S2)**: Demonstrates drinking water extraction, household use, and sewage flow.
3. **Wastewater Treatment (S3)**: Shows the journey of wastewater from households to treatment plants and rivers.
4. **Storm (S4)**: Illustrates heavy rain, street flooding, and the use of storm basins.
5. **Rainwater Harvesting (S5)**: Demonstrates rainwater collection, storage, and use for irrigation and household purposes.

## Future Improvements

- Add more detailed comments in the code for easier maintenance.
- Implement error handling and recovery mechanisms.
- Consider adding a display for providing textual information to visitors.
- Explore the possibility of adding more interactive elements or sensors.

This project provides an educational and engaging way to learn about the urban water cycle and water management strategies.