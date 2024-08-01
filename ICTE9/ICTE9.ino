// Filename:    ICTE9.ino
// Author:      Christopher Andrade (2221525), Theo Favour (2169814)
// Date:        07/31/2024
// Description: This file utilizes a binary semaphore to ensure shared resource synchronization (counter variable) across tasks.

// Pin definitions for clear hardware interfacing
#define LED_PIN 2
#define BUZZER_PIN 3 

// Task handles for managing tasks
TaskHandle_t ledTaskHandle;
TaskHandle_t buzzerTaskHandle;
TaskHandle_t numberTaskHandle;

// Semaphore Handle
SemaphoreHandle_t semaphoreHandle;

// Shared resource
volatile int sharedCounter;

// Task to toggle an LED
void ledTask(void *arg) {
  while (true) {
    if (xSemaphoreTake(semaphoreHandle, portMAX_DELAY) == pdPASS) {  // Take semaphore, wait indefinitely
      // Toggle LED from high to Low at 500 ms intervals
      digitalWrite(LED_PIN, HIGH);
      vTaskDelay(500 / portTICK_PERIOD_MS);
      digitalWrite(LED_PIN, LOW);
      vTaskDelay(500 / portTICK_PERIOD_MS);

      sharedCounter++;                                    // Modify shared resource

      xSemaphoreGive(semaphoreHandle);                    // Release semaphore
      vTaskDelay(5/portTICK_PERIOD_MS);                  // vTaskDelay to allow for other tasks of same priority to Take semaphore
    }
  }
}

// Task to generate a buzzer tone
void buzzerTask(void *arg) {
  while (true) {
    if (xSemaphoreTake(semaphoreHandle, portMAX_DELAY) == pdPASS) {  // Take semaphore, wait indefinitely
      tone(BUZZER_PIN, 1000);                             // Start buzzer
      vTaskDelay(250 / portTICK_PERIOD_MS);
      noTone(BUZZER_PIN);                                 // Stop buzzer
      vTaskDelay(250 / portTICK_PERIOD_MS);

      sharedCounter++;                                    // Modify shared resource

      xSemaphoreGive(semaphoreHandle);                    // Release semaphore
      vTaskDelay(5/portTICK_PERIOD_MS);                  // vTaskDelay to allow for other tasks of same priority to Take semaphore
    }
  }
}

// Task to print numbers to the serial monitor
void numberTask(void *arg) {
  while (true) {
    if (xSemaphoreTake(semaphoreHandle, portMAX_DELAY) == pdPASS) {  // Take semaphore, wait indefinitely
      Serial.println(sharedCounter);                      // Print the current value of sharedCounter
      vTaskDelay(1500 / portTICK_PERIOD_MS);

      xSemaphoreGive(semaphoreHandle);                    // Release semaphore
      vTaskDelay(5/portTICK_PERIOD_MS);                  // vTaskDelay to allow for other tasks of same priority to Take semaphore
    }
  }
}

void setup() {
  // Serial monitor initialization
  Serial.begin(115200);
  while(!Serial){}
  
  // Initializing of Output pins
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Creation and intial releasing of semaphore
  semaphoreHandle = xSemaphoreCreateBinary();
  xSemaphoreGive(semaphoreHandle);  

  // Create tasks and set their priorities
  xTaskCreate(ledTask, "LED Task"       , 1024, NULL, 1, &ledTaskHandle);
  xTaskCreate(buzzerTask, "Buzzer Task" , 1024, NULL, 1, &buzzerTaskHandle);
  xTaskCreate(numberTask, "Number Task" , 1024, NULL, 1, &numberTaskHandle);
}

void loop() {
  // The loop is intentionally left empty as all functionality is handled by the FreeRTOS tasks.
}