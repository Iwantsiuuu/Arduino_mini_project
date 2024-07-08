#define sensorInputPin 33
#define RL_ 7.5

float VRL, Rs, Ro = 11.26, vcc = 3.3, ratio, data_12_bit = 4094.0;

void setup() {
  pinMode(sensorInputPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  VRL = analogRead(sensorInputPin) * (vcc / data_12_bit);
  Rs = ((vcc / VRL) - 1) * (RL_);
  ratio = Rs / Ro;
  float valSensor = 1.144997421 * pow(ratio, -0.21687423);
  Serial.printf("Data sensor = %0.2f\r\n",valSensor);
  delay(1000);
}