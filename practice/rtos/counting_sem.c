#include <Arduino_FreeRTOS.h>
#include <semphr.h>

static SemaphoreHandle_t gPool;

static void Worker(void* pv);

void setup() {
  Serial.begin(115200);

  // Max = 3, start = 3
  gPool = xSemaphoreCreateCounting(3, 3);

  xTaskCreate(Worker, "W1", 192, (void*)1, 1, nullptr);
  xTaskCreate(Worker, "W2", 192, (void*)2, 1, nullptr);
  xTaskCreate(Worker, "W3", 192, (void*)3, 1, nullptr);
  xTaskCreate(Worker, "W4", 192, (void*)4, 1, nullptr);
}

void loop() {}

static void Worker(void* pv) {
  int id = (int)(intptr_t)pv;

  for (;;) {
    xSemaphoreTake(gPool, portMAX_DELAY); // acquire permit
    Serial.print("Worker "); Serial.print(id);
    Serial.println(" got resource");

    vTaskDelay(pdMS_TO_TICKS(800));       // simulate work

    Serial.print("Worker "); Serial.print(id);
    Serial.println(" releasing resource");
    xSemaphoreGive(gPool);                // release permit

    vTaskDelay(pdMS_TO_TICKS(200));
  }
}
