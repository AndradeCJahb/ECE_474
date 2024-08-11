/**
 * @file Lab4Part2.ino
 * @brief Outputs a current and averaged lightlevel to an LCD screen, plays a sound if the light level is outside a threshold.
 *
 * This file contains the code for a system that reads the light level from a photoresistor and outputs the current 
 * light level and a simple moving average to an LCD screen. If the light level is outside a threshold, 
 * a sound is played on a buzzer. Additionally, the system calculates and prints prime numbers from 2 to 5000.
 * 
 * @date 2024-07-31
 * @authors Christopher Andrade (2221525), Theo Favour (2169814)
 */

#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define BUZZER_PIN 3                        /**< Macro to reference buzzer pin D3 */

LiquidCrystal_I2C lcd(0x27, 16, 2);         /**< Initialization of LCD to display counter task */

// Task handles
TaskHandle_t LightDetectorTaskHandle;       /**< Handle to reference light detector task */
TaskHandle_t LightLevelDisplayTaskHandle;   /**< Handle to reference light level display task */
TaskHandle_t AnomalyAlarmTaskHandle;        /**< Handle to reference anomaly alarm task */
TaskHandle_t PrimeCalculationTaskHandle;    /**< Handle to reference prime calculation task */

// Semaphore handle
SemaphoreHandle_t semaphoreHandle;          /**< Handle to reference binary semaphore */

// Shared resources
volatile int SMA;                           /**< Simple moving average of light level */
volatile int lightLevel;                    /**< Current light level */


/**
 * @brief Setup function to initialize serial monitor, LCD screen, FreeRTOS tasks and external LED pins.
 *
 * This function initializes the serial monitor, LCD screen, FreeRTOS tasks and external LED pin. It also 
 * initializes the pins for the photoresistor and buzzer.
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
    
    // Initialize the LEDC PWM channel
    ledcSetup(0, 1000, 8);  // Channel 0, 1 kHz frequency, 8-bit resolution
    ledcAttachPin(BUZZER_PIN, 0);


    // Create FreeRTOS tasks
    xTaskCreatePinnedToCore(LightDetectorTask, "Light Detection Task",  2048, NULL, 1, &LightDetectorTaskHandle,     0);
    xTaskCreatePinnedToCore(LightLevelDisplayTask, "LCD Task",          2048, NULL, 1, &LightLevelDisplayTaskHandle, 0);
    xTaskCreatePinnedToCore(AnomalyAlarmTask, "Anomaly Alarm Task",     2048, NULL, 1, &AnomalyAlarmTaskHandle,      1);
    xTaskCreatePinnedToCore(PrimeCalculationTask, "Prime Task",         2048, NULL, 1, &PrimeCalculationTaskHandle,  1);
}

/**
 * @brief Task to read light level from photoresistor and calculate simple moving average.
 *
 * This function reads the light level from the photoresistor and calculates the simple moving average of 
 * the light level if semaphore is available.
 */
void LightDetectorTask(void *arg) {
    int SMAData[5] = {0};

    while(1) {
        // Read the light level from the photoresistor and store 5 measurements for SMA calculation.
        for(int i = 0; i < 5; i++) {
            lightLevel = analogRead(A0);
            SMAData[i] = lightLevel;
        }

        if (xSemaphoreTake(semaphoreHandle, portMAX_DELAY) == pdPASS) {
            // Calculate the simple moving average of the light level
            SMA = (SMAData[0] + SMAData[1] + SMAData[2] + SMAData[3] + SMAData[4]) / 5;

            xSemaphoreGive(semaphoreHandle);
            vTaskDelay(5/portTICK_PERIOD_MS); // Short delay to allow for proper semaphore handoff
        }
    }
}

/**
 * @brief Task to display light level and simple moving average on LCD screen.
 *
 * This function displays the current light level and simple moving average on the LCD screen if both a 
 * semaphore is available and the lightlevels or average has changed.
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

                // Update the previous light level and SMA.
                prevLightLevel = lightLevel;
                prevSMA = SMA;
            }

            xSemaphoreGive(semaphoreHandle);
            vTaskDelay(5/portTICK_PERIOD_MS); // Short delay to allow for proper semaphore handoff
        }
    }
}

/**
 * @brief Task to play a sound on the buzzer if the light level is outside a threshold.
 *
 * This function plays a sound on the buzzer if both a semaphore is available and the light level is outside a threshold.
 */
void AnomalyAlarmTask(void *arg) {
    // Initialize timer and boolean variable for buzzer
    bool buzz = false;
    hw_timer_t* timer = timerBegin(0, 80, true);

    while(1) {
        if(buzz) {
            // Play a sound on the buzzer if the light level is outside a threshold.
            for (int i = 0; i < 3; i++) {
                timerRestart(timer);

                // Play a sound on the buzzer for 1 second
                ledcWriteTone(0, 1000);
                while(timerRead(timer) < 1000000); 
                
                // Wait for 2 more seconds
                ledcWriteTone(0, 0);
                while(timerRead(timer) < 3000000); 
            }
            
            // Reset the buzzer flaga
            buzz = false;
        } else if (xSemaphoreTake(semaphoreHandle, portMAX_DELAY) == pdPASS) {
            if (SMA < 2000 || SMA > 3000) {
                buzz = true; // Set boolean flag to activate buzzer in the next loop iteration
            }

            xSemaphoreGive(semaphoreHandle);
            vTaskDelay(5/portTICK_PERIOD_MS); // Short delay to allow for proper semaphore handoff
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
        // Loop from 2 to 5000
        for(int i = 2; i <= 5000; i++){
            bool isPrime = true;

            // Check if the number has any divisors
            for(int j = 2; j <= i/2; j++){
                if(i % j == 0) {
                    isPrime = false;
                    break;
                }
            }

            // Print the number no divisors were found
            if(isPrime) Serial.println(i);
        }
    }
}

void loop() {}