/*
* Created: 2 Juli 2024
*Author: Ridwan Bahari
*/

#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>

#define SENSOR_STORE (2)
#define SENSOR_PICK_UP (3)
#define DETECTED (0)
#define EMPTY (0)

#define LED_GREEN (8)
#define LED_RED (4)
#define LED_WHITE (5)
#define LED_DEFAULT (6)
#define BUTTON_OFF_LED (7)
// INPUT
#define SS_PIN (10)
#define RST_PIN (9)
#define DELAY_RFID_SYS (100)
#define DELAY_COUNT_SYS (2000)
#define serial_buffer_len (80)

char serial_buffer[serial_buffer_len];

uint8_t BUTTON_COUNT = 0;
uint8_t flag_ID = 0;
uint8_t BUTTON_DATA;

uint32_t dataTest = 0;
uint32_t COUNT_ = 0;
uint32_t RFID_CARD = 0;
uint32_t prev_time_delay_rfid = 0;
uint32_t prev_time_delay_count = 0;

int buttonState;             // the current reading from the input pin
int lastButtonState = HIGH;  // the previous reading from the input pin

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

String input_uart = "";

static bool flag_store = false;
static bool flag_pick_up = false;

LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C address 0x27, 16 column and 2 rows
MFRC522 rfid(SS_PIN, RST_PIN);

// Init array that will store new NUID
byte nuidPICC[4];
void setup() {
  Serial.begin(9600);

  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_WHITE, OUTPUT);
  pinMode(LED_DEFAULT, OUTPUT);

  pinMode(BUTTON_OFF_LED, INPUT_PULLUP);
  pinMode(SENSOR_STORE, INPUT_PULLUP);
  pinMode(SENSOR_PICK_UP, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(SENSOR_STORE), store_intterupt_callback, RISING);
  attachInterrupt(digitalPinToInterrupt(SENSOR_PICK_UP), pick_up_interrupt_callback, RISING);

  lcd.init();  // initialize the lcd
  lcd.backlight();

  SPI.begin();      // Init SPI bus
  rfid.PCD_Init();  // Init MFRC522
  Serial.println(F("Scan RFID NUID..."));

  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("APLIKASI");
  lcd.setCursor(2, 1);
  lcd.print("PENYIMPANAN");

  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_WHITE, LOW);
  digitalWrite(LED_DEFAULT, HIGH);
  delay(1000);
}

void loop() {

  if (millis() < prev_time_delay_rfid)
    prev_time_delay_rfid = 0;

  if (millis() < prev_time_delay_count)
    prev_time_delay_count = 0;

  if (millis() - prev_time_delay_rfid > DELAY_RFID_SYS) {
    prev_time_delay_rfid = millis();

    if (Serial.available() > 0) {
      if (read_serial_input(Serial.read(), serial_buffer, serial_buffer_len) > 0) {
        input_uart = String(serial_buffer);
      }
    }

    BUTTON_DATA = digitalRead(BUTTON_OFF_LED);

    readRFID();
    debounce_button(BUTTON_DATA);
    led_state();
  }

  if (millis() - prev_time_delay_count > DELAY_COUNT_SYS) {
    prev_time_delay_count = millis();
    store_and_pickUp();
  }
}

void readRFID() {
  // Look for new card
  if (!rfid.PICC_IsNewCardPresent())
    return;
  // Verify if the NUID has been readed
  if (!rfid.PICC_ReadCardSerial())
    return;

  // Store NUID into nuidPICC array
  for (byte i = 0; i < 4; i++) {
    nuidPICC[i] = rfid.uid.uidByte[i];
  }

  Serial.print(F("RFID tag: "));
  RFID_CARD = passing_data(rfid.uid.uidByte, rfid.uid.size);
  Serial.println(RFID_CARD);
  // }

  // Halt PICC
  rfid.PICC_HaltA();
  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}

uint32_t passing_data(byte *buffer, byte bufferSize) {
  uint32_t data_;
  data_ = (uint32_t)((uint32_t)(buffer[0]) << 24 | (uint32_t)(buffer[1]) << 16 | (uint32_t)(buffer[2]) << 8 | (uint32_t)(buffer[3]));
  return data_;
}

void led_state() {

  if (RFID_CARD == 1423265828 || input_uart == "1423265828")
    flag_ID = 1;

  else if (RFID_CARD == 382821947 || input_uart == "382821947")
    flag_ID = 2;

  else if (RFID_CARD == 1206265902 || input_uart == "1206265902")
    flag_ID = 3;

  if (flag_ID == 1) {
    Serial.print("Data UART1: ");
    Serial.println(input_uart);
    digitalWrite(LED_DEFAULT, LOW);
    digitalWrite(LED_GREEN, HIGH);
    input_uart = "";
    flag_ID = 0;
    RFID_CARD = 0;
  } else if (flag_ID == 2) {
    Serial.print("Data UART2: ");
    Serial.println(input_uart);
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_DEFAULT, LOW);
    input_uart = "";
    flag_ID = 0;
    RFID_CARD = 0;
  } else if (flag_ID == 3) {
    Serial.print("Data UART3: ");
    Serial.println(input_uart);
    digitalWrite(LED_WHITE, HIGH);
    digitalWrite(LED_DEFAULT, LOW);
    input_uart = "";
    flag_ID = 0;
    RFID_CARD = 0;
  }
}

void debounce_button(uint8_t data_) {
  // If the switch changed, due to noise or pressing:
  if (data_ != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {

    // if the button state has changed:
    if (data_ != buttonState) {
      buttonState = data_;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == LOW) {
        digitalWrite(LED_DEFAULT, HIGH);
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_RED, LOW);
        digitalWrite(LED_WHITE, LOW);
        input_uart = "";
        flag_ID = 0;
        RFID_CARD = 0;
      }
    }
  }
  lastButtonState = data_;
}

void store_intterupt_callback() {
  flag_store = true;
}

void pick_up_interrupt_callback() {
  flag_pick_up = true;
}

void store_and_pickUp() {
  char buff[10];

  if (flag_store == true) {
    flag_store = false;
    COUNT_ += 1;
  }

  if (flag_pick_up == true) {
    flag_pick_up = false;
    if (COUNT_ != EMPTY) {
      COUNT_ -= 1;
    }
  }

  sprintf(buff, "%d Barang", COUNT_);

  lcd.clear();                   // clear display
  lcd.setCursor(0, 0);           // move cursor to   (0, 0)
  lcd.print("BARANG TERSEDIA");  // print message at (2, 1)
  lcd.setCursor(2, 1);           // move cursor to   (2, 1)
  lcd.print(buff);               // print message at (0, 0)
}

// funtion to read user serial input
int read_serial_input(int read_ch, char *buffer, int len) {
  static int pos = 0;
  int rpos;
  if (read_ch > 0) {
    switch (read_ch) {
      case '\r':  // Ignore CR
        break;
      case '\n':  // Return on new-line
        rpos = pos;
        pos = 0;  // Reset position index ready for next time
        return rpos;
      default:
        if (pos < len - 1) {
          buffer[pos++] = read_ch;
          buffer[pos] = 0;
        }
    }
  }
  return 0;
}