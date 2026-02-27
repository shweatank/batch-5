/*
  The original code:
  https://microcontrollerslab.com/use-freertos-arduino/
  Modified by Barbu Vulc!
  January 4th, 2023
*/
//RTOS = Real-time operating system
//FreeRTOS site: https://www.freertos.org/

//FreeRTOS library:
#include <Arduino_FreeRTOS.h>
//Variables for buzzer & first 3 LEDs!
const int buzzer = 7;
const int LED1 = 8;      //Red LED
const int LED2 = 9;      //Yellow LED
const int LED3 = 10;     //Green LED
/*
 * I named this 'extender' (see below) because I can optionally...
 * ...put a 4th task to this LED! Now is task-free! :))
 */
const int extender = 11; //Blue LED

void setup() {
  Serial.begin(9600);
  //LEDs' initialization!
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(extender, OUTPUT);

  //Buzzer initialization!
  pinMode(buzzer, OUTPUT);

  /*
    Create 3 tasks with labels 'Task_1', 'Task_2' and 'Task_3' and
    assign the priority as 1, 2 and 3 respectively.
  */
  //'Neutral_Task' - the task-free function!
  xTaskCreate(Task_1, "Task no. 1!", 100, NULL, 1, NULL);
  xTaskCreate(Task_2, "Task no.  2!", 100, NULL, 2, NULL);
  xTaskCreate(Task_3, "Task no.  3!", 100, NULL, 3, NULL);
  xTaskCreate(Neutral_Task, "Neutral_Task!", 100, NULL, 0, NULL);
}

//The following function is Task1. We display the task label on Serial monitor.
static void Task_1(void* pvParameters) {
  while (1) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(extender, LOW);
    tone(buzzer, 200, 1000 / portTICK_PERIOD_MS);
    Serial.println(F("Task no. 1!"));
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

//Task 2
static void Task_2(void* pvParameters) {
  while (1) {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, LOW);
    digitalWrite(extender, LOW);
    tone(buzzer, 300, 1100 / portTICK_PERIOD_MS);
    Serial.println(F("Task no. 2!"));
    vTaskDelay(1100 / portTICK_PERIOD_MS);
  }
}

//Task 3
static void Task_3(void* pvParameters) {
  while (1) {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, HIGH);
    digitalWrite(extender, LOW);
    tone(buzzer, 400, 1200 / portTICK_PERIOD_MS);
    Serial.println(F("Task no. 3!"));
    vTaskDelay(1200 / portTICK_PERIOD_MS);
  }
}

//Task 4 (the last one)!
//This is an extension which can be task-free!
static void Neutral_Task(void* pvParameters) {
  while (1) {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(extender, HIGH);
    tone(buzzer, 500, 1300);
    Serial.println(F("Neutral_Task"));
    delay(1300);
  }
}

//We don't need to use "loop" function here!
void loop() {}

