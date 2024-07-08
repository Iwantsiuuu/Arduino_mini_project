#include <SPI.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <Adafruit_Fingerprint.h>
//************************************************************************
//Fingerprint scanner Pins
#define Finger_Rx 13  //D5
#define Finger_Tx 15  //D6
// #define Solenoid 10

//************************************************************************
SoftwareSerial mySerial(Finger_Rx, Finger_Tx);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

int relayPin = 10;
int buzzerPin = 15;

void setup() {
  pinMode(relayPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  digitalWrite(relayPin, LOW);
  digitalWrite(buzzerPin, LOW);

  finger.begin(57600);
  Serial.begin(115200);


  while (!Serial);
  delay(100);

  Serial.println("Fingerprint Door Lock");
  delay(3000);

  if (finger.verifyPassword()) {
    Serial.println("Fingerprint Sensor Connected");
    delay(3000);
  }

  else {
    Serial.println("Unable to find Sensor");
    delay(3000);
    Serial.println("Check Connections");

    while (1) {
      delay(1);
    }
  }
}

void loop()  // run over and over again
{
  getFingerprintIDez();
  delay(50);  //don't need to run this at full speed.
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) {
    Serial.println("Waiting For Valid Finger");
    return -1;
  }

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) {
    Serial.println("Messy Image Try Again");
    delay(3000);
    return -1;
  }

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) {
    Serial.println("Not Valid Finger");
    delay(3000);
    return -1;
  }

  digitalWrite(relayPin, HIGH);
  digitalWrite(buzzerPin, HIGH);
  delay(3000);

  digitalWrite(relayPin, LOW);
  digitalWrite(buzzerPin, LOW);
  delay(3000);

  Serial.println("Door Unlocked Welcome");

  return finger.fingerID;
}
