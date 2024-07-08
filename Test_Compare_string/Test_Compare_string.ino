#include <string.h>

char array[10];

char *string1 = "1020202";

void setup() {
  Serial.begin(9600);
}
void loop() {

  strcpy(&array[2], string1);

  int index = -1;
  for (int i = 0; i < 10; i++) {
    int result = strcmp(&array[i], string1);
    Serial.printf("Hasil compare= %d\n",result);
    if (result == 0) {
      index = i;
      break;
    }
  }

  if (index != -1) {
    Serial.printf("Value string1 like array[%d]\r\n", index);
  } else {
    // The value of string1 was not found in the array
    Serial.println("Value string1 not found at array");
  }
  delay(1000);
}
