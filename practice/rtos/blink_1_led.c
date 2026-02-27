#include <Arduino_FreeRTOS.h>
const int LED1 = 8;      //Red LED
/*
 * I named this 'extender' (see below) because I can optionally...
 * ...put a 4th task to this LED! Now is task-free! :))
 */
const int extender = 11; //Blue LED

void setup() {
  Serial.begin(9600);
  //LEDs' initialization!
  pinMode(LED1, OUTPUT);

  //'Neutral_Task' - the task-free function!
  xTaskCreate(Neutral_Task, "Neutral_Task!", 100, NULL, 0, NULL);*/
  xTaskCreate(T1,"ledon_task",100,NULL,1,NULL);
  xTaskCreate(T2,"ledoff_task",100,NULL,2,NULL);
}

//The following function is Task1. We display the task label on Serial monitor.
static void T1(void *pvParamaters)
{
  while(1){
  digitalWrite(LED1,HIGH);
  Serial.println(F("led_on_task!!"));
  vTaskDelay(2000/portTICK_PERIOD_MS);}
}
static void T2(void *pvParamaters)
{
  while(1){
  digitalWrite(LED1,LOW);
  Serial.println(F("led_off_task!!"));
  vTaskDelay(2200/portTICK_PERIOD_MS);
  }
}
void loop(){}
