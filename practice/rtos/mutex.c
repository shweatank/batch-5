#include <Arduino_FreeRTOS.h>
#include <semphr.h>

static SemaphoreHandle_t gSerialMutex;

static void TaskA(void* pv);
static void TaskB(void* pv);

void setup() {
  Serial.begin(115200);

  gSerialMutex = xSemaphoreCreateMutex();

  xTaskCreate(TaskA, "A", 192, nullptr, 1, nullptr);
  xTaskCreate(TaskB, "B", 192, nullptr, 1, nullptr);
}

void loop() {}

static void SafePrint(const char* msg) {
  // Wait forever for the mutex
  xSemaphoreTake(gSerialMutex, portMAX_DELAY);
  Serial.println(msg);
  xSemaphoreGive(gSerialMutex);
}

static void TaskA(void* pv) {
  (void)pv;
  for (;;) {
    SafePrint("TaskA: hello");
    vTaskDelay(pdMS_TO_TICKS(400));
  }
}

static void TaskB(void* pv) {
  (void)pv;
  for (;;) {
    SafePrint("TaskB: world");
    vTaskDelay(pdMS_TO_TICKS(700));
  }
}

