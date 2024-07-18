#include <Arduino_FreeRTOS.h>

TaskHandle_t TaskButtonHandle;
TaskHandle_t TaskDisplayHandle;

#define TASK_PRIORITY 6
#define STACK_SIZE    256

// define two Tasks for DigitalRead & AnalogRead
void TaskDigitalRead(void *pvParameters);
void TaskDisplay(void *pvParameters);

// the setup function runs once when you press reset or power the board
void setup() {

  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);

  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB, on LEONARDO, MICRO, YUN, and other 32u4 based boards.
  }

  // Now set up two Tasks to run independently.
  xTaskCreate(TaskDigitalRead, "DigitalRead", (STACK_SIZE*2), NULL, (TASK_PRIORITY - 1), &TaskButtonHandle);
  xTaskCreate(TaskDisplay, "Display", (STACK_SIZE*2), NULL, (TASK_PRIORITY - 2), &TaskDisplayHandle);

  vTaskStartScheduler();

}

void loop() {
  // Empty. Things are done in Tasks.
}

/*--------------------------------------------------*/
/*---------------------- Tasks ---------------------*/
/*--------------------------------------------------*/

void TaskDigitalRead(void *pvParameters)  // This is a Task.
{

  for (;;)  // A Task shall never return or exit.
  {
    Serial.println("Hallo Task button");
    vTaskDelay(100);
  }
}

void TaskDisplay(void *pvParameters)  // This is a Task.
{

  for (;;) {
    Serial.println("Hallo Task Display");
    vTaskDelay(200);
  }
}
