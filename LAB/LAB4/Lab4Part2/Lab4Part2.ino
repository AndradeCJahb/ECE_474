/**
 * @file Lab4Part2.ino
 * @brief 
 *
 * 
 * 
 * @date 2024-07-31
 * @authors Christopher Andrade (2221525), Theo Favour (2169814)
 */

#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define BUZZER_PIN 3

LiquidCrystal_I2C lcd(0x27, 16, 2);  /**< Initialization of LCD to display counter task */

// Task handles
TaskHandle_t LightDetectorTaskHandle;
TaskHandle_t LightLevelDisplayTaskHandle;
TaskHandle_t AnomalyAlarmTaskHandle;
TaskHandle_t PrimeCalculationTaskHandle;

// Semaphore Handle
SemaphoreHandle_t semaphoreHandle;

// Shared resources
volatile int SMA;
volatile int lightLevel;


/**
 * @brief 
 *
 * 
 */
void setup() {
    // Initialize Serial monitor, LCD screen, and external LED pin
    Serial.begin(115200);
    while(!Serial);

    // Initialize LCD screen
    lcd.init();
    lcd.backlight();

    // Initialize pins for photoresistor and buzzer
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(A0, INPUT);

    // Create a binary semaphore
    semaphoreHandle = xSemaphoreCreateBinary();
    xSemaphoreGive(semaphoreHandle);  

    xTaskCreatePinnedToCore(LightDetectorTask, "Light Detection Task",  2048, NULL, 1, &LightDetectorTaskHandle,     0);
    xTaskCreatePinnedToCore(LightLevelDisplayTask, "LCD Task",          2048, NULL, 1, &LightLevelDisplayTaskHandle, 0);
    xTaskCreatePinnedToCore(AnomalyAlarmTask, "Anomaly Alarm Task",     2048, NULL, 1, &AnomalyAlarmTaskHandle,      1);
    xTaskCreatePinnedToCore(PrimeCalculationTask, "Prime Task",         2048, NULL, 1, &PrimeCalculationTaskHandle,  1);
}

/**
 * @brief 
 *
 * 
 */
void LightDetectorTask(void *arg) {
    int SMAData[5] = {0};

    while(1) {
        // Read light level from the photoresistor.
        for(int i = 0; i < 5; i++) {
            lightLevel = analogRead(A0);
            SMAData[i] = lightLevel;
        }

        if (xSemaphoreTake(semaphoreHandle, portMAX_DELAY) == pdPASS) {
            // Calculate the simple moving average and update variables.

            SMA = (SMAData[0] + SMAData[1] + SMAData[2] + SMAData[3] + SMAData[4]) / 5;

            xSemaphoreGive(semaphoreHandle);
            vTaskDelay(5/portTICK_PERIOD_MS);
        }
    }
}

/**
 * @brief 
 *
 * 
 */
void LightLevelDisplayTask(void *arg) {
    int prevLightLevel = 0;
    int prevSMA = 0;

    while(1) {
        if (xSemaphoreTake(semaphoreHandle, portMAX_DELAY) == pdPASS) {
            // Check if light level or SMA has changed
            if (lightLevel != prevLightLevel || SMA != prevSMA) {
                // Update the LCD with the new light level and SMA.
                lcd.setCursor(0, 0);
                lcd.print("Light: ");
                lcd.print(lightLevel);
                lcd.print("  ");

                lcd.setCursor(0, 1);
                lcd.print("SMA: ");
                lcd.print(SMA);
                lcd.print("  ");

                prevLightLevel = lightLevel;
                prevSMA = SMA;
            }

            xSemaphoreGive(semaphoreHandle);
            vTaskDelay(5/portTICK_PERIOD_MS);
        }
    }
}

/**
 * @brief 
 *
 * 
 */
void AnomalyAlarmTask(void *arg) {
    while(1) {
        if (xSemaphoreTake(semaphoreHandle, portMAX_DELAY) == pdPASS) {
            if (SMA < 1000 || SMA > 2000) {
                // Play a sound on the buzzer if the light level is outside the threshold.
                tone(BUZZER_PIN, 1000);
                vTaskDelay(500/portTICK_PERIOD_MS);
                noTone(BUZZER_PIN);
            }

            xSemaphoreGive(semaphoreHandle);
            vTaskDelay(5/portTICK_PERIOD_MS);
        }
    }
}

/**
 * @brief Task to calculate and print prime numbers.
 * 
 * This task loops from 2 to 5000 and checks if each number is prime.
 * If a number is prime, it is printed to the serial monitor.
 * 
 */
void PrimeCalculationTask(void *arg) {
    while(1) {
        for(int i = 2; i <= 5000; i++){
            bool isPrime = true;

            for(int j = 2; j <= i/2; j++){
                if(i % j == 0){
                    isPrime = false;
                    break;
                }
            }
            if(isPrime) Serial.println(i);
        }
    }
}

void loop() {}