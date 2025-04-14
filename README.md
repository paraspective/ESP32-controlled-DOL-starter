# ESP32-Controlled DOL Starter with Monitoring and Auto-Protection

An ESP32-based Direct-On-Line (DOL) starter controller for 3-phase AC motors. This system provides real-time current monitoring across all phases, displays data on an LCD, and includes optional built-in protections like overcurrent cutoff and phase imbalance detection.

## Features
- Overcurrent protection with configurable threshold
- Phase imbalance detection
- Automatic restart if current drops below safe threshold
- Real-time current display on I2C LCD
- Advanced calibration and zero-drift monitoring

## Hardware Used
- ESP32 Dev Module
- 3x ACS712 Current Sensors (30A/20A)
- PC817 opto-isolator (or similar) for relay control
- 16x2 I2C LCD
- Optional: Buttons for manual reset or override

## Files
- `main.ino`: Core logic for sensing, protection, and display
- `calibration.ino`: Use this first to calibrate sensor offsets and analyze noise

## Pin Connections
| Component     | ESP32 Pin |
|---------------|-----------|
| ACS712 L1     | GPIO34    |
| ACS712 L2     | GPIO35    |
| ACS712 L3     | GPIO36    |
| Relay Control | GPIO4     |
| I2C SDA       | GPIO21    |
| I2C SCL       | GPIO22    |

## Getting Started with calibrTION
1. Upload `calibration.ino`, run it with zero current to get baseline voltages.
2. Save those zero values and, if needed, update them in the main sketch.
3. Upload `main.ino` and observe the LCD for real-time feedback.
4. Adjust `OVERCURRENT_TRIP`, `RESTART_THRESHOLD`, and `IMBALANCE_LIMIT` as per your application.

## Getting Started with main
1. Clone this repo
2. Open `src/main.ino` in Arduino IDE
3. Install required libraries:
   - `LiquidCrystal_I2C`
4. Choose your ESP32 board and port
5. Upload the code and power the board

## Notes
- Use proper current isolation and protection before connecting to high-power loads.
- The ACS712 has some drift and noise—always calibrate!
