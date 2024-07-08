#define PIN_A_MUX D5
#define PIN_B_MUX D6
#define PIN_LOGIC D7

uint8_t logicMux = 0;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_A_MUX, OUTPUT);
  pinMode(PIN_B_MUX, OUTPUT);
  pinMode(PIN_LOGIC, OUTPUT);
  digitalWrite(PIN_LOGIC, HIGH);
}
unsigned long prev_time = 0;

void loop() {
  // put your main code here, to run repeatedly:

  if (millis() - prev_time > 1000) {
    prev_time = millis();
    logicMux++;
    Serial.println(logicMux);

    if (logicMux == 1) logicMux_sound(LOW, LOW);
    else if (logicMux == 2) logicMux_sound(LOW, HIGH);
    else if (logicMux == 3) logicMux_sound(HIGH, LOW);
    else if (logicMux == 4) {
      logicMux_sound(HIGH, HIGH);
      logicMux = 0;
    }
  }
}

void logicMux_sound(uint8_t logic_pin_a_mux, uint8_t logic_pin_b_mux) {
  digitalWrite(PIN_A_MUX, logic_pin_a_mux);
  digitalWrite(PIN_B_MUX, logic_pin_b_mux);
}