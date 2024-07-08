#include <Wire.h>
#include <LiquidCrystal_I2C.h>  // Library for LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int sampleWindow = 50;  // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

#define SENSOR_PIN A0
#define PIN_QUIET D4
#define PIN_MODERATE D5
#define PIN_LOUD D6

void setup() {
  pinMode(SENSOR_PIN, INPUT);  // Set the signal pin as input
  pinMode(PIN_QUIET, OUTPUT);
  pinMode(PIN_MODERATE, OUTPUT);
  pinMode(PIN_LOUD, OUTPUT);

  digitalWrite(PIN_QUIET, LOW);
  digitalWrite(PIN_MODERATE, LOW);
  digitalWrite(PIN_LOUD, LOW);

  Serial.begin(115200);
  lcd.begin(16,2,16);
  // Turn on the backlight.
  lcd.backlight();
  lcd.clear();
}


void loop() {
  unsigned long startMillis = millis();  // Start of sample window

  float peakToPeak = 0;  // peak-to-peak level

  unsigned int signalMax = 0;     //minimum value
  unsigned int signalMin = 4095;  //maximum value

  // collect data for 50 mS
  while (millis() - startMillis < sampleWindow) {
    sample = analogRead(SENSOR_PIN);  //get reading from microphone
    if (sample < 1024)                // toss out spurious readings
    {
      if (sample > signalMax) {
        signalMax = sample;  // save just the max levels
      } else if (sample < signalMin) {
        signalMin = sample;  // save just the min levels
      }
    }
  }

  peakToPeak = signalMax - signalMin;           // max - min = peak-peak amplitude
  int db = map(peakToPeak, 20, 900, 49.5, 90);  //calibrate for deciBels
  Serial.println(db);
  // lcd.setCursor(0, 0);
  // lcd.print("Loudness: ");
  // lcd.print(db);
  // lcd.print("dB");

  // if (db <= 60) {
  //   lcd.setCursor(0, 1);
  //   lcd.print("Level: Quite");
  //   digitalWrite(PIN_QUIET, HIGH);
  //   digitalWrite(PIN_MODERATE, LOW);
  //   digitalWrite(PIN_LOUD, LOW);
  // } else if (db > 60 && db < 85) {
  //   lcd.setCursor(0, 1);
  //   lcd.print("Level: Moderate");
  //   digitalWrite(PIN_QUIET, LOW);
  //   digitalWrite(PIN_MODERATE, HIGH);
  //   digitalWrite(PIN_LOUD, LOW);
  // } else if (db >= 85) {
  //   lcd.setCursor(0, 1);
  //   lcd.print("Level: High");
  //   digitalWrite(PIN_QUIET, LOW);
  //   digitalWrite(PIN_MODERATE, LOW);
  //   digitalWrite(PIN_LOUD, HIGH);
  // }

  delay(10);
  // lcd.clear();
}