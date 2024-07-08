#include <LiquidCrystal_I2C.h>

#define SENSOR_STORE    (2)
#define SENSOR_PICK_UP  (3)
#define DETECTED        (0)
#define EMPTY           (0)
#define DELAY_DEVICE    (2000)

LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C address 0x27, 16 column and 2 rows

uint32_t dataTest = 0;
uint32_t COUNT_ = 0;
uint32_t prev_time_delay = 0;

char buff[10];

static bool flag_store = false;
static bool flag_pick_up = false;

void setup() {
  Serial.begin(9600);

  pinMode(SENSOR_STORE, INPUT_PULLUP);
  pinMode(SENSOR_PICK_UP, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(SENSOR_STORE), store_intterupt_callback, RISING);
  attachInterrupt(digitalPinToInterrupt(SENSOR_PICK_UP), pick_up_interrupt_callback, RISING);

  lcd.init();  // initialize the lcd
  lcd.backlight();
}

void loop() {
  dataTest = store_and_pickUp();

  sprintf(buff, "%d Items", dataTest);

  lcd.clear();                   // clear display
  lcd.setCursor(0, 0);           // move cursor to   (0, 0)
  lcd.print("ITEMS AVAILABLE");  // print message at (2, 1)
  lcd.setCursor(2, 1);           // move cursor to   (2, 1)
  lcd.print(buff);               // print message at (0, 0)

  delay(1000);                   // display the above for two seconds
}

void store_intterupt_callback() {
  flag_store = true;
}

void pick_up_interrupt_callback() {
  flag_pick_up = true;
}

uint32_t store_and_pickUp() {

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

  return COUNT_;
}
