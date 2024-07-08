#define ID_PIN 15
#define MODE_PIN 5

uint8_t vall = 0;

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(9600);
  pinMode(ID_PIN, INPUT_PULLUP);
  pinMode(MODE_PIN, INPUT_PULLUP);

  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  uint data = digitalRead(ID_PIN);
  uint data1 = digitalRead(MODE_PIN);

  // Serial.println(data);
  if (data != 1) {
    vall++;
    Serial.print("Data: ");
    Serial.println(vall);
  }
  if (data1 != 1) {
    vall--;
    Serial.print("Data: ");
    Serial.println(vall);
  }
  delay(200);
}
