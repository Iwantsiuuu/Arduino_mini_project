const uint8_t inputNode = 4;
const uint8_t hiddenNode = 4;
const uint8_t outputNode = 1;

const float weight_1[hiddenNode][inputNode] = {
  { 0.1650, 0.5009, 0.8083, -0.7957 },
  { 2.8969, -2.9054, 0.1244, 0.0356 },
  { 3.0390, -0.8884, -0.3069, -0.2094 },
  { -2.9047, 1.2746, -0.5498, -0.4430 }
};

const float bias_1[] = {
  -2.5863,
  -0.2289,
  0.0085,
  0.0860
};

const float weight_2[] = {
  -0.4546,
  7.7966,
  3.9379,
  -3.7011
};

const float bias_2 = -0.7282;

const float xOffset[inputNode] = { 13.0, 14.0, 21.0, 11.0 };
const float gain[inputNode] = { 0.00851, 0.00847, 0.00897, 0.00866 };
const float yMin[inputNode] = { -1.0 };

float input_data_testing[inputNode] = { 26.0, 204.0, 88.0, 242.0 };
float output = 0.0;
float hiddenLayer[hiddenNode];
float norm_input[inputNode];
float accumtion;
int x, y, i, j;

// Prototype function
// float mapFloat(float x, float in_min, float in_max, float out_min, float out_max);
void nn_compute();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:

  nn_compute();
  delay(1000);
}

// float mapFloat(float x, float in_min, float in_max, float out_min, float out_max){
//   return (x-in_min) * (out_max - out_min)/(in_max - in_min) + out_min;
// }

void nn_compute() {
  for (i = 0; i < hiddenNode; i++) {
    norm_input[i] = input_data_testing[i] - xOffset[i];
    norm_input[i] = norm_input[i] * gain[i];
    norm_input[i] = norm_input[i] + yMin[i];
  }

  for (i = 0; i < hiddenNode; i++) {
    hiddenLayer[i] = 0;
  }

  for (i = 0; i < hiddenNode; i++) {
    for (j = 0; j < hiddenNode; j++) {
      hiddenLayer[i] += norm_input[j] * weight_1[i][j];
    }
    hiddenLayer[i] = (2.0 / (1.0 + exp(-(2 * hiddenLayer[i] + bias_1[i])))) - 1;
  }

  accumtion = 0;
  for (i = 0; i < hiddenNode; i++) {
    accumtion += hiddenLayer[i] * weight_2[i];
  }
  output = 1.0 / (1.0 + exp(-(accumtion + bias_2)));
  Serial.println(output);
}