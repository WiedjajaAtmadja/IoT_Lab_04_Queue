#include <Arduino.h>
#define LED_GREEN   4
#define LED_YELLOW  5
#define LED_RED    18

QueueHandle_t xQueue;

void taskProduce(void *pvParameters) {
  int i = 0;
  while (1) {
    Serial.printf("==> Produce: %d \n", i); fflush(stdout);
    xQueueSend(xQueue, &i, portMAX_DELAY);
    i++;
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void taskConsume(void *pvParameters) {
  int i;
  while (1) {
    xQueueReceive(xQueue, &i, portMAX_DELAY);
    Serial.printf("<== Consume: %d \n", i); fflush(stdout);
    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_RED, LOW);

  xQueue = xQueueCreate(3, sizeof(int));
  xTaskCreatePinnedToCore(taskProduce, "taskProduce", configMINIMAL_STACK_SIZE+1024, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(taskConsume, "taskConsume", configMINIMAL_STACK_SIZE+1024, NULL, 1, NULL, 1);
  Serial.println("System ready");
}

void loop() {
  vTaskDelay(0);
}