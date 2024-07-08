#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL6hSq58i9l"
#define BLYNK_TEMPLATE_NAME "Test"
#define BLYNK_AUTH_TOKEN "AAXjM49XXN2JuJvnZNmXaiBsH3KZMIPu"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_Fingerprint.h>

#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
SoftwareSerial mySerial(2, 3);

#else
#define mySerial Serial2
#endif

#define SS_PIN 21
#define RST_PIN 22

#define SOLENOID_PIN (25)
#define PIN_ID (5)
#define PIN_MOD (15)

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
MFRC522 mfrc522(SS_PIN, RST_PIN);

BlynkTimer timer;

char ssid[] = "iwants";
char pass[] = "244466666";

String id = "";
String word_sendV0, finger_setupV1;

bool finger_config = false;
bool enroll_mod = false;
bool delete_mod = false;
bool close_enroll = false;
bool enter_mod = false;

uint8_t mode = 0, ID = 0, enter_mode = 0;

void myTimer() {
  Blynk.virtualWrite(V0, id);
  id = "";
}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  pinMode(PIN_ID, INPUT_PULLUP);
  pinMode(PIN_MOD, INPUT_PULLUP);
  pinMode(SOLENOID_PIN, OUTPUT_OPEN_DRAIN);
  digitalWrite(SOLENOID_PIN, HIGH);
  
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(300L, myTimer);
}
void loop() {
  Blynk.run();
  timer.run();
  readmfrc522();
}
void readmfrc522() {
  String tag;
  if (!mfrc522.PICC_IsNewCardPresent()) {
    Serial.println("RFID_READ");
    return;
  }
  if (mfrc522.PICC_ReadCardSerial()) {
    for (byte i = 0; i < 4; i++) {
      tag += mfrc522.uid.uidByte[i];
    }
    Serial.println(tag);
    if (tag == "211771437") {
      // Serial.println(word_sendV0);
      // digitalWrite(SOLENOID_PIN, LOW);

      // word_sendV0 = "Open the Solenoid(RFID)";
      // Blynk.virtualWrite(V0, word_sendV0);
      Serial.println("Open the Solenoid(RFID)");
      delay(5000);

    } else {
      // word_sendV0 = "Access Denied!";

      // Blynk.virtualWrite(V0, word_sendV0);
      Serial.println("Access Denied!");

      delay(100);
    }

    tag = "";

    // digitalWrite(SOLENOID_PIN, HIGH);
    // word_sendV0 = "Close the Solenoid";
    // Blynk.virtualWrite(V0, word_sendV0);
    Serial.println("Close");

    delay(1000);

    // word_sendV0 = "Please tap your RFID Card or Finger";
    // Serial.println(word_sendV0);

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }
}

void RFID() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  id = "";
  Serial.print("UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  id = content.substring(1);
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      // Serial.println("No finger detected");
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

  if (p == FINGERPRINT_OK) {
    Serial.println(word_sendV0);
    digitalWrite(SOLENOID_PIN, LOW);
    word_sendV0 = "Open the Solenoid(Finger)";
    Blynk.virtualWrite(V0, word_sendV0);

    delay(5000);
  }
  // found a match!
  Serial.print("Found ID #");
  Serial.print(finger.fingerID);
  Serial.print(" with confidence of ");
  Serial.println(finger.confidence);

  // matched_tag_flag = true;
  digitalWrite(SOLENOID_PIN, HIGH);
  word_sendV0 = "Close the Solenoid";
  Blynk.virtualWrite(V0, word_sendV0);
  delay(1000);

  word_sendV0 = "Please tap your RFID Card or Finger";

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

  Serial.println(word_sendV0);
  digitalWrite(SOLENOID_PIN, LOW);
  word_sendV0 = "Open the Solenoid";
  Blynk.virtualWrite(V0, word_sendV0);

  delay(5000);
  // found a match!
  Serial.print("Found ID #");
  Serial.print(finger.fingerID);
  Serial.print(" with confidence of ");
  Serial.println(finger.confidence);

  // matched_tag_flag = true;
  digitalWrite(SOLENOID_PIN, HIGH);
  word_sendV0 = "Close the Solenoid";
  Blynk.virtualWrite(V0, word_sendV0);
  delay(1000);

  word_sendV0 = "Please tap your RFID Card or Finger";

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
  enter_mode = 0;
  mode = 0;
  ID = 0;
  return p;
}

uint8_t getFingerprintEnroll() {
  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #");
  Serial.println(ID);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
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

  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID ");
  Serial.println(ID);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
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
  Serial.print("Creating model for #");
  Serial.println(ID);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID ");
  Serial.println(ID);
  p = finger.storeModel(ID);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }
  word_sendV0 = "ID Stored";
  Blynk.virtualWrite(V0, word_sendV0);
  delay(500);
  word_sendV0 = "Please tap your RFID Card or Finger";
  return true;
}