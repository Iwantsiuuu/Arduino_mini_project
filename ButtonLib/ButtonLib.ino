/*New blynk app project
   Home Page
*/
#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL6dSq2v9Jw"
#define BLYNK_TEMPLATE_NAME "Notify RFID"
#define BLYNK_AUTH_TOKEN "zavqiN20gOKU_IxYBXLjoh0BLy3aVfzO"
//Include the library files
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>


char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "iwants";     //Enter your WIFI name
char pass[] = "244466666";  //Enter your WIFI password

//Get the button value
BLYNK_WRITE(V0) {
  digitalWrite(10, param.asInt());
}

typedef struct {
  uint8_t State = LOW, lastState;
  uint8_t lastButtonState;
  uint8_t output, lastOut;

  unsigned long holdTime = 500, DBInterval = 50, RO_Time;
  unsigned long time, duration, HeldTime;
  unsigned long lastDebounceTime;
} button_t;

button_t btn;

int LED1 = 11;
int LED2 = 10;
int button = 5;

boolean LED1State = false;
boolean LED2State = false;

long buttonTimer = 0;
long longPressTime = 250;

boolean buttonActive = false;
boolean longPressActive = false;

void setup() {
  //Set the LED pin as an output pin
  Serial.begin(9600);
  pinMode(LED2, OUTPUT);
  // pinMode(9, INPUT_PULLUP);
  pinMode(button, INPUT_PULLUP);
  //Initialize the Blynk library
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
}
long milisPrev = 0;
void loop() {
  //Run the Blynk library
  if (millis() - milisPrev >= 100) {
    Blynk.run();
    button_app();
      // Serial.println(var);
    // if (digitalRead(9) != 1)
    // Serial.println("Pin9");
    milisPrev = millis();
  }
}

void button_app(){
  	if (digitalRead(button) == HIGH) {

		if (buttonActive == false) {

			buttonActive = true;
			buttonTimer = millis();

		}

		if ((millis() - buttonTimer > longPressTime) && (longPressActive == false)) {

			longPressActive = true;
			LED1State = !LED1State;
			digitalWrite(LED1, LED1State);
      Serial.println("longPress");

		}

	} else {

		if (buttonActive == true) {

			if (longPressActive == true) {

				longPressActive = false;

			} else {

				LED2State = !LED2State;
				digitalWrite(LED2, LED2State);

			}

			buttonActive = false;

		}

	}
}

uint8_t button_cb() {
  btn.output = 0;
  uint8_t button_ = digitalRead(5);
  if (button_ != btn.lastButtonState) {
    btn.lastDebounceTime = millis();
    btn.lastButtonState = button_;
    btn.time = millis();
  }
  unsigned long RO_Time = millis();
  if (RO_Time < btn.time)
    btn.time = RO_Time;

  while ((millis() - btn.time) <= btn.duration) {
    if (button_ != btn.lastState) {
      if (button_ == !btn.State) {
        if ((millis() - btn.lastDebounceTime) >= btn.DBInterval) {
          btn.output++;
          btn.lastButtonState = millis();
        }
      }
      btn.lastState = digitalRead(5);
    }
    button_ = digitalRead(5);
  }
  if (button_ == btn.State && button_ == btn.lastDebounceTime)
    if ((btn.HeldTime = (millis() - btn.time)) > btn.holdTime)
      btn.output = 100;

      btn.lastOut = btn.output;
      return btn.output;
}