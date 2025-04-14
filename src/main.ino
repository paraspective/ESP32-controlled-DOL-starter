#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <math.h>  // For abs()

// === Hardware Config ===
const byte ACS712_PINS[] = {34, 35, 36};  // L1, L2, L3 analog inputs
const int PC817_PIN = 4;                  // Digital output to opto-isolator

const float SENSITIVITY[] = {0.066, 0.066, 0.100}; // mV/A for 30A, 30A, 20A sensors

// === Protection Settings ===
const float OVERCURRENT_TRIP = 25.0;       // Overcurrent threshold (Amps)
const float RESTART_THRESHOLD = 20.0;      // Current must drop below this to auto-reset
const float IMBALANCE_LIMIT = 2.0;         // Max allowable difference between phases

// === LCD Config ===
LiquidCrystal_I2C lcd(0x27, 16, 2);        // 16x2 I2C LCD

void setup() {
  pinMode(PC817_PIN, OUTPUT);
  digitalWrite(PC817_PIN, HIGH);  // Start with DOL enabled

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("DOL Protection");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();

  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
}

void loop() {
  float currents[3];
  float avgCurrent = 0;
  bool trip = false;
  String tripMessage = "";

  for (int i = 0; i < 3; i++) {
    currents[i] = getCurrent(i);
    avgCurrent += currents[i];

    if (currents[i] > OVERCURRENT_TRIP) {
      trip = true;
      tripMessage = "PHASE " + String(i + 1) + " OVER!";
    }

    for (int j = 0; j < i; j++) {
      if (abs(currents[i] - currents[j]) > IMBALANCE_LIMIT) {
        trip = true;
        tripMessage = "IMBALANCE L" + String(j + 1) + "-L" + String(i + 1);
      }
    }
  }

  avgCurrent /= 3.0;

  if (trip) {
    digitalWrite(PC817_PIN, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PROTECTION TRIP");
    lcd.setCursor(0, 1);
    lcd.print(tripMessage);
  } else {
    if (avgCurrent < RESTART_THRESHOLD) {
      digitalWrite(PC817_PIN, HIGH);
    }

    lcd.setCursor(0, 0);
    lcd.print("L1:");
    lcd.print(currents[0], 1);
    lcd.print(" L2:");
    lcd.print(currents[1], 1);

    lcd.setCursor(0, 1);
    lcd.print("L3:");
    lcd.print(currents[2], 1);
    lcd.print(" Avg:");
    lcd.print(avgCurrent, 1);
  }

  delay(500);
}

float getCurrent(int phase) {
  int raw = analogRead(ACS712_PINS[phase]);
  float voltage = raw * (3.3 / 4095.0);
  float current = (voltage - 1.65) / SENSITIVITY[phase];
  return current;
}
