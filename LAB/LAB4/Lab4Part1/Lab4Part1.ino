/**
 * @file Lab4Part1.ino
 * @brief Schedules and carries out 3 distinct tasks using SRTF algorithm in FreeRTOS
 *
 * This file contains code to simultaneously flash an external LED, display an incrementing counter 1-20 on an LCD screen, and sequentially print the alphabet to the serial monitor. 
 * It uses the shortest remaining time first algorithm, implemented using a highest-priority scheduling task since FreeRTOS doesn't include SRTF natively.
 * 
 * @date 2024-07-31
 * @authors Christopher Andrade (2221525), Theo Favour (2169814)
 */

#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define LED_PIN 2  /**< Macro to reference external LED pin D2 */

TaskHandle_t scheduleTasksHandle;   /**< Handle to reference Task Scheduler Task */
TaskHandle_t ledTaskHandle;         /**< Handle to reference off-board LED Task */
TaskHandle_t alphabetTaskHandle;    /**< Handle to reference alphabet printing Task */
TaskHandle_t counterTaskHandle;     /**< Handle to reference 1-20 counter Task */

LiquidCrystal_I2C lcd(0x27, 16, 2);  /**< Initialization of LCD to display counter task */

// Total times for tasks
const TickType_t ledTaskExecutionTime = 510 / portTICK_PERIOD_MS;           /**< 500 ms timer for LED task + allowances for code execution */ 
const TickType_t counterTaskExecutionTime = 20100 / portTICK_PERIOD_MS;     /**< 20 second timer for counter task + allowances for code execution */
const TickType_t alphabetTaskExecutionTime = 26100 / portTICK_PERIOD_MS;    /**< 26 second timer for alphabet task + allowances for code execution */

// Remaining Execution Times
volatile TickType_t remainingLedTime = ledTaskExecutionTime;            /**< volatile int to contain calculated remaining execution time of LED task, initialized to full Execution time */
volatile TickType_t remainingCounterTime = counterTaskExecutionTime;    /**< volatile int to contain calculated remaining execution time of 1-20 counter task, initialized to full Execution time  */
volatile TickType_t remainingAlphabetTime = alphabetTaskExecutionTime;  /**< volatile int to contain calculated remaining execution time of alphabet printing task, initialized to full Execution time  */

/**
 * @brief Task which blinks an external LED on and off in 500 millisecond intervals
 *
 * This function blinks an external LED on and off in 500 millisecond intervals. The LED is connected to pin D2 on the ESP32. The task is executed every 500 milliseconds.
 */
void ledTask(void *arg) {
    TickType_t startTick, totalTime; // Ticktype variables to calculate start time & ending time

    while(1) {
        startTick = xTaskGetTickCount();
        
        // Toggle the LED pin every 500 milliseconds
        digitalWrite(LED_PIN, !digitalRead(LED_PIN));
        vTaskDelay(500/portTICK_PERIOD_MS);

        // Calculate total time taken to execute the current iteration of the task
        totalTime = xTaskGetTickCount() - startTick;

        // Update remaining time for the task, or reset it if the task has completed
        if(remainingLedTime > totalTime) {
            remainingLedTime -= totalTime;
        } else {
            remainingLedTime = ledTaskExecutionTime; 
        }
    }
}

/**
 * @brief Task which increments a counter from 1 to 20 on an LCD screen
 *
 * This function increments a counter from 1 to 20 on an LCD screen, in the form "counter: (count)". The counter is updated every second and the display is cleared upon reaching 20.
 */
void counterTask(void *arg) {
    TickType_t startTick, totalTime;    // Ticktype variables to calculate start time & ending time
    
    int count = 1;
    while(1) {
        startTick = xTaskGetTickCount();
        
        // Print the counter value in the correct format to the LCD screen
        lcd.setCursor(0, 0);
        lcd.print("count: ");
        lcd.setCursor(7, 0);
        lcd.print(count);

        vTaskDelay(1000/portTICK_PERIOD_MS);
        
        // Clear the LCD screen after reaching 20
        if (count == 20) {
            count = 0;
            lcd.clear();
        }
        count++;
        
        totalTime = xTaskGetTickCount() - startTick;    // Calculate total time taken to execute the current iteration of the task

        // Update remaining time for the task, or reset it if the task has completed
        if(remainingCounterTime > totalTime) {
            remainingCounterTime -= totalTime;
        } else {
            remainingCounterTime = counterTaskExecutionTime; 
        }
    }
}

/**
 * @brief Task which prints the alphabet sequentially to the serial monitor
 *
 *  This function prints the alphabet sequentially to the serial monitor, separated by commas. The task is executed every second, therefore the entire alphabet is printed in 26 second intervals.
 */
void alphabetTask(void *arg) {
    TickType_t startTick, totalTime; // Ticktype variables to calculate start time & ending time
    char alpha = 'A';   // Character variable to store the current alphabet character
    Serial.println();
    
    while(1) {
        startTick = xTaskGetTickCount();
        
        Serial.print(alpha);   
        
        // Print a comma after each alphabet character, except for 'Z', which resets to the beginning of the alphabet
        if (alpha != 'Z') {
            Serial.print(',');
            alpha++;
        } else {
            Serial.println();
            alpha = 'A';
        }

        vTaskDelay(1000/portTICK_PERIOD_MS);
        
        totalTime = xTaskGetTickCount() - startTick;    // Calculate total time taken to execute the current iteration of the task

        // Update remaining time for the task, or reset it if the task has completed
        if(remainingAlphabetTime > totalTime) {
            remainingAlphabetTime -= totalTime;
        } else {
            remainingAlphabetTime = alphabetTaskExecutionTime; 
        }
    }
}

/**
 * @brief High-priority scheduling task based on SRTF algorithm
 *
 *  This function schedules the three tasks based on the Shortest Remaining Time First (SRTF) algorithm. It sets the priority of the tasks based on the remaining time for each task to execute.
 *  Since it's priority 4, it will always activate first after every FreeRTOS interrupt tick.
 */
void scheduleTasks(void *arg) {
    while (1) {
        // Set the priority of the tasks based on the remaining time for each task to execute, with the shortest time having the highest priority
        if (remainingLedTime <= remainingCounterTime && remainingLedTime <= remainingAlphabetTime) {
            vTaskPrioritySet(ledTaskHandle, 2);
            if (remainingCounterTime <= remainingAlphabetTime) {
                vTaskPrioritySet(counterTaskHandle, 1); 
                vTaskPrioritySet(alphabetTaskHandle, 0);
            } else {
                vTaskPrioritySet(counterTaskHandle, 0); 
                vTaskPrioritySet(alphabetTaskHandle, 1);
            }
        } else if(remainingCounterTime <= remainingLedTime && remainingCounterTime <= remainingAlphabetTime) {
            vTaskPrioritySet(counterTaskHandle,2);
            if (remainingLedTime <= remainingAlphabetTime) {
                vTaskPrioritySet(alphabetTaskHandle, 0);
                vTaskPrioritySet(ledTaskHandle, 1);
            } else {
                vTaskPrioritySet(alphabetTaskHandle, 1);
                vTaskPrioritySet(ledTaskHandle, 0);
            }
        } else {
            vTaskPrioritySet(alphabetTaskHandle, 2);
            if(remainingLedTime <= remainingCounterTime) {
                vTaskPrioritySet(ledTaskHandle, 1);
                vTaskPrioritySet(counterTaskHandle, 0);
            } else {
                vTaskPrioritySet(ledTaskHandle, 0);
                vTaskPrioritySet(counterTaskHandle, 1);
            }
        }
    }
}

/**
 * @brief Setup function to initialize Serial monitor, LCD screen, and FreeRTOS tasks
 *
 *  This function initializes the tasks and peripherals used in the program. It initializes the LCD screen, sets up the external LED pin, and creates the tasks for the LED, counter, and alphabet functions.
 */
void setup() {
    // Initialize Serial monitor, LCD screen, and external LED pin
    Serial.begin(115200);
    while(!Serial);
    lcd.init();
    lcd.backlight();
    pinMode(LED_PIN, OUTPUT);

    // Create tasks for LED, counter, alphabet, and scheduler functions
    xTaskCreatePinnedToCore(scheduleTasks, "SRTF Handler"   , 2048, NULL, 4, &scheduleTasksHandle,0);
    xTaskCreatePinnedToCore(alphabetTask, "Alphabet Task"   , 2048, NULL, 0, &alphabetTaskHandle,0);
    xTaskCreatePinnedToCore(counterTask, "Counter Task"     , 2048, NULL, 0, &counterTaskHandle,0);
    xTaskCreatePinnedToCore(ledTask, "LED Task"             , 2048, NULL, 0, &ledTaskHandle,0);
}

void loop() {}
