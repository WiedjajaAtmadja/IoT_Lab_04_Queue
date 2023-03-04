#include <Arduino.h>
#define LED_GREEN    4
#define LED_YELLOW   5
#define LED_RED     18
#define PUSH_BUTTON 23

QueueHandle_t xQueue;

IRAM_ATTR void isrPushButton()
{
  static int nCount=0;
  nCount++;
  xQueueSend(xQueue, &nCount, portMAX_DELAY);
}

void taskConsume(void *pvParameters) {
  int i;
  while (1) {
    xQueueReceive(xQueue, &i, portMAX_DELAY);
    digitalWrite(LED_GREEN, HIGH);
    Serial.printf("Process: %d \n", i); fflush(stdout);
    vTaskDelay(50 / portTICK_PERIOD_MS);
    digitalWrite(LED_GREEN, LOW);
    vTaskDelay(950 / portTICK_PERIOD_MS);
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

  xQueue = xQueueCreate(10, sizeof(int));
  xTaskCreatePinnedToCore(taskConsume, "taskConsume", 4096, NULL, 1, NULL, 1);
  attachInterrupt(digitalPinToInterrupt(PUSH_BUTTON), isrPushButton, FALLING);

  Serial.println("System ready");
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  vTaskDelay(50 / portTICK_PERIOD_MS);
  digitalWrite(LED_BUILTIN, LOW);
  vTaskDelay(950 / portTICK_PERIOD_MS);
}