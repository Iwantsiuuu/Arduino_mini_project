#include "esp32-hal-gpio.h"
void loop() {  // run over and over again

  if (Serial.available()) {

    enrollBuff = Serial.read();
    if (enrollBuff == 'C') {
      fConfig = true;
    } else if (enrollBuff == 'D') {
      Serial.println("Please type in the ID # (from 1 to 127) you want to delete...");
      id = readnumber();
      deleteFingerprint(id);
    }
  }
  if (fConfig) {
    Serial.println("Ready to enroll a fingerprint!");
    Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
    id = readnumber();

    if (id == 130) {  // exit enroll id mode
      Serial.print("exit enroll#");
      fConfig = false;

    } else {
      Serial.print("Enrolling ID #");
      Serial.println(id);
      while (!getFingerprintEnroll())
        ;
    }
  }

  else {
    getFingerprintID();
    readmfrc522();
  }
}
//global
bool enroll_mod = false;
bool delete_mod = false;
bool close_enroll = false;
bool enter_mod = false;

uint8_t data_pin = 0;
uint8_t ID_INPUT = 0;

BLYNK_WRITE(V1) {
  if (param.asInt() == 1)
    enroll_mod = true;
  else
    enroll_mod = false;
}

BLYNK_WRITE(V2) {
  if (param.asInt() == 1)
    delete_mod = true;
  else
    delete_mod = false;
}

BLYNK_WRITE(V3) {
  if (param.asInt() == 1)
    close_enroll = true;
  else
    close_enroll = false;
}

BLYNK_WRITE(V4) {
  if (param.asInt() == 1)
    enter_mod = true;
  else
    enter_mod = false;
}

void input_id() {
  ID_INPUT++;
}

//Program in loop
if (enroll_mod == true) {
  data_pin = digitalRead(ID_PIN);
  if()

  if (enter_mod == true) {

    if (close_enroll == true)
      enroll_mod = false;

    else
      while (!getFingerprintEnroll())
        ;
  }
}
if (delete_mod == true) {
  data_pin = digitalRead(ID_PIN);

  if (enter_mod == true)
    deleteFingerprint(data_pin);
}