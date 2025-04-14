# ESP32-Controlled DOL Starter with Monitoring and Auto-Protection

An ESP32-based Direct-On-Line (DOL) starter controller for 3-phase AC motors. This system provides real-time current monitoring across all phases, displays data on an LCD, and includes optional built-in protections like overcurrent cutoff and phase imbalance detection.

## Features
- Real-time current monitoring via LCD
- Auto trip on high current or phase imbalance (configurable)
- Automatic restart when safe current levels resume
- Compact hardware footprint using ESP32 and I2C LCD

## Hardware Used
- ESP32 Dev Module
- 3x ACS712 Current Sensors (30A & 20A)
- PC817 Opto-isolator (or equivalent relay trigger)
- 16x2 LCD with I2C backpack

## Pin Connections
| Component     | ESP32 Pin |
|---------------|-----------|
| ACS712 L1     | GPIO34    |
| ACS712 L2     | GPIO35    |
| ACS712 L3     | GPIO36    |
| Relay Control | GPIO4     |
| I2C SDA       | GPIO21    |
| I2C SCL       | GPIO22    |

## Getting Started
1. Clone this repo
2. Open `src/main.ino` in Arduino IDE
3. Install required libraries:
   - `LiquidCrystal_I2C`
4. Choose your ESP32 board and port
5. Upload the code and power the board
