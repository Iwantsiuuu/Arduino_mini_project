
uint8_t State = LOW, lastState;
uint8_t lastButtonState = true;
uint8_t output, lastOut;

unsigned long holdtimeT = 500, DBInterval = 50, RO_timeT;
unsigned long timeT, duration, HeldtimeT;
unsigned long lastDebouncetimeT;

int LED1 = 9;
int LED2 = 10;
int button = 5;

boolean LED1State = false;
boolean LED2State = false;

long buttontimeTr = 0;
long longPresstimeT = 250;

boolean buttonActive = false;
boolean longPressActive = false;

void setup() {
  //Set the LED pin as an output pin
  Serial.begin(9600);
  pinMode(LED2, OUTPUT);
  // pinMode(9, INPUT_PULLUP);
  pinMode(button, INPUT_PULLUP);
  //Initialize the Blynk library
}
long milisPrev = 0;
void loop() {
  //Run the Blynk library
  if (millis() - milisPrev >= 500) {
    uint8_t outbtn = button_cb(button);
    // Serial.println(outbtn);
    milisPrev = millis();
  }
}

uint8_t button_cb(uint8_t btn) {

  output = 0;
  uint8_t button_ = digitalRead(btn);

  if (button_ != lastButtonState) {

    lastDebouncetimeT = millis();
    // Serial.println("Button detected");
    lastButtonState = button_;
    timeT = millis();
  }

  unsigned long RO_timeT = millis();
  if (RO_timeT < timeT)
    timeT = RO_timeT;

  while ((millis() - timeT) <= duration) {
    // Serial.println("While Pressed");

    if (button_ != lastState) {
      // Serial.println("Button!=lastState");

      if (button_ == !State) {
        // Serial.println("Button == true");

        if ((millis() - lastDebouncetimeT) <= DBInterval) {
          // Serial.println("Pressed");

          output++;
          lastButtonState = millis();
        }
      }
      lastState = digitalRead(btn);
    }
    button_ = digitalRead(btn);
  }

  if (button_ == State && button_ == lastButtonState) {
    // Serial.println("Check Long pressed");

    if ((HeldtimeT = (millis() - timeT)) > holdtimeT) {
      // Serial.println("Long Press detected");

      output = 100;
    }
  }
  Serial.print("Output btn ");
  Serial.println(output);
  // lastOut = output;
  return output;
}