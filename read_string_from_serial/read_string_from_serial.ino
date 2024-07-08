//Serial buffer
#define serial_buffer_len 80
char serial_buffer[serial_buffer_len];
String user_input = "";

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    if (read_serial_input(Serial.read(), serial_buffer, serial_buffer_len) > 0) {
      user_input = String(serial_buffer);
    }
  }
  if (user_input == "hallo")
    Serial.println("Correct input:" + user_input);
  else if (user_input == "")
    Serial.println("Please input");
  else Serial.println("Wrong input");


  delay(100);
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
