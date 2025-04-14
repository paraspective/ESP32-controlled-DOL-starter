#include <driver/adc.h>

// Hardware Configuration
const byte ACS712_PINS[] = {34, 35, 36};  // L1, L2, L3
const int SENSOR_TYPES[] = {30, 30, 20};  // 30A, 30A, 20A
const float VREF = 3.3;
const int ADC_RESOLUTION = 4095;

// Advanced Calibration
float zeroVoltages[3] = {0};
float noiseThresholds[3] = {0.05, 0.05, 0.05}; // Acceptable noise (A)

void setup() {
  Serial.begin(115200);
  
  // Configure ESP32 ADC for better accuracy
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
  
  Serial.println("\n=== Advanced ESP32 Calibration ===");
  calibrateSensors();
  analyzeNoise();
  Serial.println("\nCalibration Complete!");
}

void loop() {
  monitorZeroDrift();
}

void calibrateSensors() {
  Serial.println("\nStarting 2-stage calibration...");
  
  for(int phase=0; phase<3; phase++) {
    Serial.printf("\nCalibrating L%d (ACS712-%dA):\n", phase+1, SENSOR_TYPES[phase]);
    
    // Stage 1: Quick initial calibration
    zeroVoltages[phase] = getAverageReading(phase, 500, 5);
    
    // Stage 2: Precision calibration
    zeroVoltages[phase] = getAverageReading(phase, 2000, 20);
    
    Serial.printf("Final Zero: %.4fV\n", zeroVoltages[phase]);
  }
}

float getAverageReading(int phase, int samples, int delayMs) {
  Serial.printf("Taking %d samples...\n", samples);
  
  // Discard first 100 readings (warm-up)
  for(int i=0; i<100; i++) {
    analogRead(ACS712_PINS[phase]);
    delay(1);
  }
  
  // Collect real samples
  double sum = 0;
  for(int i=0; i<samples; i++) {
    sum += analogRead(ACS712_PINS[phase]);
    delay(delayMs);
    
    // Show progress
    if(i % (samples/10) == 0) Serial.print(".");
  }
  
  return (sum / samples) * (VREF / ADC_RESOLUTION);
}

void analyzeNoise() {
  Serial.println("\nAnalyzing sensor noise...");
  
  for(int phase=0; phase<3; phase++) {
    float maxNoise = 0;
    for(int i=0; i<100; i++) {
      float current = getCurrent(phase);
      maxNoise = max(maxNoise, abs(current));
      delay(10);
    }
    noiseThresholds[phase] = maxNoise * 1.5; // 50% safety margin
    Serial.printf("L%d Noise Threshold: ±%.4fA\n", phase+1, noiseThresholds[phase]);
  }
}

float getCurrent(int phase) {
  float voltage = (analogRead(ACS712_PINS[phase]) * (VREF/ADC_RESOLUTION)) - zeroVoltages[phase];
  return voltage / (SENSOR_TYPES[phase] == 20 ? 0.100 : 0.066);
}

void monitorZeroDrift() {
  static unsigned long lastPrint = 0;
  
  if(millis() - lastPrint >= 1000) {
    Serial.println("\n--- Zero-Current Monitoring ---");
    for(int phase=0; phase<3; phase++) {
      float current = getCurrent(phase);
      
      Serial.printf("L%d: %+.4fA", phase+1, current);
      
      if(abs(current) > noiseThresholds[phase]) {
        Serial.print(" (WARNING: Drift detected!)");
      }
      Serial.println();
    }
    lastPrint = millis();
  }
}
