/* Read RFID Tag with RC522 RFID Reader
 *  Made by miliohm.com
 */
#define BLYNK_TEMPLATE_ID "TMPL6dSq2v9Jw"
#define BLYNK_TEMPLATE_NAME "Notify RFID"
#define BLYNK_AUTH_TOKEN "zavqiN20gOKU_IxYBXLjoh0BLy3aVfzO"

#define BLYNK_PRINT Serial

#include <SPI.h>
#include <MFRC522.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
// #include <BlynkSimpleEsp8266.h>

#define LED_PIN 10
/*
PIN RC522 Using ESP32
SDA   = D21
SCL    = D18
MOSI = D23
MISO = D19
RQ     = NC
GND  = GND
RST    = D22
VDD   = 3.3V

PIN RC522 Using ESP8266
SDA   = D4
SCL    = D5
MOSI = D7
MISO = D6
RQ     = NC
GND  = GND
RST    = D3
VDD   = 3.3V
*/

constexpr uint8_t RST_PIN = D22;  // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = D21;   // Configurable, see typical pin layout above

MFRC522 rfid(SS_PIN, RST_PIN);  // Instance of the class
MFRC522::MIFARE_Key key;

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "iwants";     // type your wifi name
char pass[] = "244466666";  // type your wifi password

BlynkTimer timer;

String tag;
String word_send;

bool matched_tag_flag = false;
bool not_matched_tag_flag = false;

void setup() {
  Serial.begin(9600);
  SPI.begin();      // Init SPI bus
  rfid.PCD_Init();  // Init MFRC522

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  Blynk.begin(auth, ssid, pass);
  timer.setInterval(500L, myDasboard);
  word_send = "Please tap your RFID Card";
}

uint8_t mode = 0, ID = 0, set_RFID = 0;

void loop() {
  Blynk.run();
  timer.run();
  if (Serial.available()) {
    mode = Serial.read();
    if (mode == 'R')
      set_RFID = 1;
  }
  readRFID();
}

void readRFID() {

  if (!rfid.PICC_IsNewCardPresent())
    return;
  if (rfid.PICC_ReadCardSerial()) {
    for (byte i = 0; i < 4; i++) {
      tag += rfid.uid.uidByte[i];
    }
    Serial.println(tag);
    if (tag == "211771437") {
      matched_tag_flag = HIGH;

      // Serial.println(word_send);

      digitalWrite(LED_PIN, LOW);
      word_send = "Open the Solenoid";
      Blynk.virtualWrite(V0, word_send);
      delay(5000);

    } else {
      not_matched_tag_flag = true;
      Serial.println("Access Denied!");
    }

    tag = "";

    digitalWrite(LED_PIN, HIGH);
    word_send = "Close the Solenoid";
    Blynk.virtualWrite(V0, word_send);
    delay(1000);

    word_send = "Please tap your RFID Card";

    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }
}

void myDasboard() {
  Blynk.virtualWrite(V0, word_send);
  Serial.println(word_send);
  word_send = "";
}

void myNotify() {
  if (matched_tag_flag == true)
    Blynk.logEvent("rfid_notify", "Accessed!\nWellCome");
  if (not_matched_tag_flag == true)
    Blynk.logEvent("rfid_notify", "Access Denied!");
}