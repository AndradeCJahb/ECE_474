#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#include <freertos/FreeRtos.h>
#include <freertos/task.h>

#define LED_PIN 2

TaskHandle_t scheduleTasksHandle;
TaskHandle_t ledTaskHandle;
TaskHandle_t alphabetTaskHandle;
TaskHandle_t counterTaskHandle;

LiquidCrystal_I2C lcd(0x27, 16, 2); // Initialize the LCD

// Total times for tasks
const TickType_t ledTaskExecutionTime = 500 / portTICK_PERIOD_MS;      // 500 ms
const TickType_t counterTaskExecutionTime = 2000 / portTICK_PERIOD_MS; // 2 seconds
const TickType_t alphabetTaskExecutionTime = 13000 / portTICK_PERIOD_MS; // 13 seconds

// Remaining Execution Times
volatile TickType_t remainingLedTime = ledTaskExecutionTime;
volatile TickType_t remainingCounterTime = counterTaskExecutionTime;
volatile TickType_t remainingAlphabetTime = alphabetTaskExecutionTime;

void ledTask(void *arg) {
    TickType_t startTick, totalTime;

    while(1) {
        startTick = xTaskGetTickCount(); 
        
        digitalWrite(LED_PIN, !digitalRead(LED_PIN));
        vTaskDelay(500/portTICK_PERIOD_MS);

        totalTime = xTaskGetTickCount() - startTick;

        if(remainingLedTime > totalTime) {
            remainingLedTime -= totalTime;
        } else {
            remainingLedTime = ledTaskExecutionTime; 
        }
    }
}

void counterTask(void *arg) {
    TickType_t startTick, totalTime;
    lcd.setCursor(0, 0);
    lcd.print("count: ");
    lcd.setCursor(7, 0);

    int count = 0;
    while(1) {
        startTick = xTaskGetTickCount();

        lcd.print(count);
        count++;
        vTaskDelay(1000/portTICK_PERIOD_MS);

        totalTime = xTaskGetTickCount() - startTick;

        if(remainingCounterTime > totalTime) {
            remainingCounterTime -= totalTime;
        } else {
            remainingCounterTime = counterTaskExecutionTime; 
        }
    }
}

void alphabetTask(void *arg) {
    TickType_t startTick, totalTime;

    while(1) {
        startTick = xTaskGetTickCount();

        for(int i = 0; i < 26; i++) {
            Serial.print((char)(i+65));
            if(i != 25) {
                Serial.print(',');
            }
        }
        Serial.println();
        vTaskDelay(1000/portTICK_PERIOD_MS);
        
        totalTime = xTaskGetTickCount() - startTick;

        if(remainingAlphabetTime > totalTime) {
            remainingAlphabetTime -= totalTime;
        } else {
            remainingAlphabetTime = alphabetTaskExecutionTime; 
        }
    }
}

void scheduleTasks(void *arg) {
    while (1) {
        if (remainingLedTime <= remainingCounterTime && remainingLedTime <= remainingAlphabetTime) {
            vTaskPrioritySet(alphabetTaskHandle, 0);
            vTaskPrioritySet(counterTaskHandle, 0);
            vTaskPrioritySet(ledTaskHandle, 2); 
        } else if(remainingCounterTime <= remainingLedTime && remainingCounterTime <= remainingAlphabetTime) {
            vTaskPrioritySet(alphabetTaskHandle, 0);
            vTaskPrioritySet(counterTaskHandle, 2);
            vTaskPrioritySet(ledTaskHandle, 0);  
        } else {
            vTaskPrioritySet(alphabetTaskHandle, 2);
            vTaskPrioritySet(counterTaskHandle, 0);
            vTaskPrioritySet(ledTaskHandle, 0); 
        }
        vTaskDelay(10/portTICK_PERIOD_MS);
    }
}

void setup() {
    Serial.begin(115200);
    lcd.init();
    lcd.backlight();
    pinMode(LED_PIN, OUTPUT);

    xTaskCreatePinnedToCore(scheduleTasks, "SRTF Handler"   , 1024, NULL, 10, &scheduleTasksHandle,0);
    xTaskCreatePinnedToCore(alphabetTask, "Alphabet Task"   , 1024, NULL, 0, &alphabetTaskHandle,0);
    xTaskCreatePinnedToCore(counterTask, "Counter Task"     , 1024, NULL, 0, &counterTaskHandle,0);
    xTaskCreatePinnedToCore(ledTask, "LED Task"             , 1024, NULL, 0, &ledTaskHandle,0);
}
void loop() {}
