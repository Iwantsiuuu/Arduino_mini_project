#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL6hSq58i9l"
#define BLYNK_TEMPLATE_NAME "Test"
#define BLYNK_AUTH_TOKEN "AAXjM49XXN2JuJvnZNmXaiBsH3KZMIPu"

#include <SPI.h>
#include <WiFi.h>
#include <MFRC522.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Adafruit_Fingerprint.h>

#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)

SoftwareSerial mySerial(2, 3);

#else
// On Leonardo/M0/etc, others with hardware serial, use hardware serial!
// #0 is green wire, #1 is white
#define mySerial Serial2

#endif

#define SS_PIN 21
#define RST_PIN 22

#define LED_PIN 2

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
MFRC522 mfrc522(SS_PIN, RST_PIN);

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "lina";
char pass[] = "B4tam2022#";

BlynkTimer timer;

String tag;
String word_send;

bool matched_tag_flag = false;
bool not_matched_tag_flag = false;

void setup() {
  Serial.begin(9600);
  SPI.begin();         // Init SPI bus
  mfrc522.PCD_Init();  // Init MFRC522

  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  Serial.println(F("Reading sensor parameters"));
  finger.getParameters();
  Serial.print(F("Status: 0x"));
  Serial.println(finger.status_reg, HEX);
  Serial.print(F("Sys ID: 0x"));
  Serial.println(finger.system_id, HEX);
  Serial.print(F("Capacity: "));
  Serial.println(finger.capacity);
  Serial.print(F("Security level: "));
  Serial.println(finger.security_level);
  Serial.print(F("Device address: "));
  Serial.println(finger.device_addr, HEX);
  Serial.print(F("Packet len: "));
  Serial.println(finger.packet_len);
  Serial.print(F("Baud rate: "));
  Serial.println(finger.baud_rate);

  finger.getTemplateCount();

  if (finger.templateCount == 0) {
    Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
  } else {
    Serial.println("Waiting for valid finger...");
    Serial.print("Sensor contains ");
    Serial.print(finger.templateCount);
    Serial.println(" templates");
  }

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  Blynk.begin(auth, ssid, pass);
  timer.setInterval(500L, myDasboard);
  word_send = "Please tap your RFID Card or Finger";
}

void loop() {
  Blynk.run();
  timer.run();

  readmfrc522();
  getFingerprintID();
  delay(50);
}

void readmfrc522() {

  if (!mfrc522.PICC_IsNewCardPresent())
    return;
  if (mfrc522.PICC_ReadCardSerial()) {
    for (byte i = 0; i < 4; i++) {
      tag += mfrc522.uid.uidByte[i];
    }
    Serial.println(tag);
    if (tag == "211771437") {
      matched_tag_flag = true;

      Serial.println(word_send);
      digitalWrite(LED_PIN, LOW);
      word_send = "Open the Solenoid";
      Blynk.virtualWrite(V0, word_send);

      delay(5000);

    } else {
      // not_matched_tag_flag = true;
      Serial.println("Access Denied!");
    }

    tag = "";

    matched_tag_flag = true;
    digitalWrite(LED_PIN, HIGH);
    word_send = "Close the Solenoid";
    Blynk.virtualWrite(V0, word_send);
    delay(1000);

    word_send = "Please tap your RFID Card or Finger";

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }
}

void myDasboard() {
  Blynk.virtualWrite(V0, word_send);
  Serial.println(word_send);
  word_send = "";
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

if (p == FINGERPRINT_OK){
  Serial.println(word_send);
  digitalWrite(LED_PIN, LOW);
  word_send = "Open the Solenoid";
  Blynk.virtualWrite(V0, word_send);

  delay(5000);
}
  // found a match!
  Serial.print("Found ID #");
  Serial.print(finger.fingerID);
  Serial.print(" with confidence of ");
  Serial.println(finger.confidence);

  matched_tag_flag = true;
  digitalWrite(LED_PIN, HIGH);
  word_send = "Close the Solenoid";
  Blynk.virtualWrite(V0, word_send);
  delay(1000);

  word_send = "Please tap your RFID Card or Finger";

  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) return -1;

  Serial.println(word_send);
  digitalWrite(LED_PIN, LOW);
  word_send = "Open the Solenoid";
  Blynk.virtualWrite(V0, word_send);

  delay(5000);
  // found a match!
  Serial.print("Found ID #");
  Serial.print(finger.fingerID);
  Serial.print(" with confidence of ");
  Serial.println(finger.confidence);

  matched_tag_flag = true;
  digitalWrite(LED_PIN, HIGH);
  word_send = "Close the Solenoid";
  Blynk.virtualWrite(V0, word_send);
  delay(1000);

  word_send = "Please tap your RFID Card or Finger";

  return finger.fingerID;
}

uint8_t deleteFingerprint(uint8_t id) {

  uint8_t p = -1;
  p = finger.deleteModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Deleted!");

  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");

  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not delete in that location");

  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");

  } else {
    Serial.print("Unknown error: 0x");
    Serial.println(p, HEX);
  }
  return p;
}