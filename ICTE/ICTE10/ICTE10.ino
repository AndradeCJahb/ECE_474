static const uint8_t queue_len = 10;  // Size of queue
static const int num_prod_tasks = 5;  // Number of producer tasks
static const int num_cons_tasks = 2;  // Number of consumer tasks
static const int num_writes = 4;      // Num times each producer writes to buf

// Globals
static SemaphoreHandle_t semaphore;  // Waits for parameter to be read
static SemaphoreHandle_t mutex;    // Lock access to Serial resource
static QueueHandle_t q;    // Send data from producer to consumer


// Producer Task Function
void producer(void *parameters) {
  for(int i = 0; i < 5; i++) {
    int toQueue = rand() % 100;
    xQueueSend(q, &toQueue, 10);
  }
  xSemaphoreGive(semaphore);

  vTaskDelete(NULL);
}

void consumer(void *parameters) {
  int sum = 0, average = 0, numReceived = 0;

  while (1) {
      int fromQueue = 0;
      if(xSemaphoreTake(semaphore, portMAX_DELAY) == pdPASS) {
        while(xQueueReceive(q, &fromQueue, portMAX_DELAY) == pdPASS) {
          sum += fromQueue;
          numReceived++;
        }
        xSemaphoreGive(semaphore);
      }

      if(numReceived >= 10 ) {
        if (xSemaphoreTake(mutex, portMAX_DELAY)) {
          average = sum/numReceived;
          Serial.print("sum: ");
          Serial.println(sum);
          Serial.print("average: ");
          Serial.println(average);
          sum = 0;
          numReceived = 0;

          xSemaphoreGive(mutex);
        }
      }
      vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void setup() {
  char task_name[12];

  Serial.begin(115200);
  while(!Serial);

  vTaskDelay(2000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Semaphore Solution---");

  semaphore = xSemaphoreCreateBinary();
  mutex = xSemaphoreCreateMutex();
  q = xQueueCreate(queue_len, sizeof(int));

  for (int i = 0; i < num_prod_tasks; i++) {
    sprintf(task_name, "Producer %i", i);
    xTaskCreatePinnedToCore(producer, task_name, 2048, NULL, 1, NULL, 0);
    xSemaphoreTake(semaphore, portMAX_DELAY);
  }

  for (int i = 0; i < num_cons_tasks; i++) {
    sprintf(task_name, "Consumer %i", i);
    xTaskCreatePinnedToCore(consumer, task_name, 2048, NULL, 1, NULL, 0);
  }

  xSemaphoreTake(mutex, portMAX_DELAY);
  Serial.println("All tasks created");
  xSemaphoreGive(mutex);
}

void loop() {}