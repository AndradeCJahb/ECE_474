/**
 * @file FinalProject_DebugOrSegfault.ino
 * @brief Final Project for EECSE 474 Embedded Systems including a couple game tasks to be completed to win or "Debug" the system.
 * 
 * The project includes a Joystick direction task and a RTC task to be completed to win the game. The game also includes a displayed timer and buzzer to indicate time remaining.
 * 
 * @date 2024-08-14
 * @authors Christopher Andrade (2221525), Theo Favour (2169814)
 */

#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SPI.h>
#include <I2C_RTC.h>

// I2C LCD and RTC initialization
static DS1307 RTC;
LiquidCrystal_I2C lcd(0x27, 16, 2);         /**< Initialization of LCD to display counter task */

// Pin definitions for joystick
const int joystick_x_pin = A0;              /**< Pin definition for joystick X coordinate */

// Task handles
TaskHandle_t GameEndTaskHandle;             /**< Handle to reference GameEndTask */ 
TaskHandle_t JoystickInputTaskHandle;       /**< Handle to reference JoystickInput */
TaskHandle_t BuzzerTimeTaskHandle;          /**< Handle to reference BuzzerTask */
TaskHandle_t JoystickGameTaskHandle;        /**< Handle to reference JoyStickGame */
TaskHandle_t SerialRTCTaskHandle;           /**< Handle to reference RTCTask */

// Queue handles
QueueHandle_t DirectionsQueue;              /**< Handle to reference Queue of generated (correct) directions */
QueueHandle_t AnalogX;                      /**< Handle to referece Queue of user inputted directions*/

// Global flags
volatile bool win = false;                  /**< Flag indicating a win, both RTC and Joystick task completion */
volatile bool joystickWin = false;          /**< Flag indicating whether the user has won the Joystick minigame */
volatile bool joystickStart = false;        /**< Flag indicating when to begin the Joystick intro sequence */
volatile bool RTCWin = false;               /**< Flag indicating whether the user has completed the RTC task */  
volatile bool lose = false;                 /**< Flag indicating whether the user has lost by timeout or misinput */
volatile bool joystickRunning = false;      /**< Flag indicating when to begin the joystick input detecting phase */

volatile int timeCountDown;                 /**< Global timeRemaining variable */

/**
 * @brief Setup function to initialize serial monitor, LCD screen, FreeRTOS Tasks/Queues, external Buzzer pins, and RTC.
 *
 * Completes various tasks to set up the program environment, Initializing both queues, all Tasks, LCD, Serial, RTC, Buzzer, Joystick, etc.
 */
void setup() {
    // Serial Monitor initialization
    Serial.begin(115200);
    while(!Serial);

    // LCD initialization
    lcd.init();
    lcd.backlight();

    // Buzzer pin initialization
    pinMode(3, OUTPUT);
    ledcSetup(0, 1000, 8);
    ledcAttachPin(3, 0);

    // Queue for Joystick game
    DirectionsQueue = xQueueCreate(8, sizeof(int));  
    AnalogX         = xQueueCreate(2, sizeof(int));

    // Core 1 tasks
    xTaskCreatePinnedToCore(JoystickGameTask, "JoystickGameTask", 2048, NULL, 1, &JoystickGameTaskHandle, 1);
    xTaskCreatePinnedToCore(JoystickProducerTask, "JoystickProducerTask", 2048, NULL, 2, NULL, 1);
    xTaskCreatePinnedToCore(JoystickInputTask, "JoystickInputTask", 2048, NULL, 1, &JoystickInputTaskHandle, 1);
    xTaskCreatePinnedToCore(SerialRTCInputTask, "RTCInputTask", 2048, NULL, 1, &SerialRTCTaskHandle, 1);

    // Core 0 tasks
    xTaskCreatePinnedToCore(GameEndTask, "GameEndTask", 2048, NULL, 1, &GameEndTaskHandle, 0);
    xTaskCreatePinnedToCore(BuzzerTimeTask, "BuzzerTimeTask", 2048, NULL, 1, &BuzzerTimeTaskHandle, 0);
    
    // RTC initialization
    RTC.setHourMode(CLOCK_H12);
    RTC.setDateTime(__TIMESTAMP__); 
}

/**
 * @brief Task to handle game timer, including win and lose conditions.
 * 
 * Detects win and lose conditions with boolean flag variables and a global countdown timer, displaying the current state of the game on the LCD
 */
void GameEndTask(void *pvParameters) {
    hw_timer_t* timer = timerBegin(0, 80, true);    // Timer 0, prescaler 80, 1MHz frequency

    while(1) {
        if(joystickWin && RTCWin) { // Win condition
            // Display winning message on LCD
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Code Debugged");
            
            win = true;
            
            vTaskDelete(NULL);
        } else {
            timeCountDown = 45 - timerRead(timer)/1000000;

            // Display time remaining on LCD
            lcd.setCursor(0, 0);
            lcd.print("time: ");
            lcd.setCursor(6, 0);
            lcd.print(timeCountDown);
            lcd.print(" ");

            // Lose condition, set by failure of joystick task or time running out
            if(timeCountDown <= 0 || lose) {
                lose = true;
                
                // Display SEG FAULT "lose" message on LCD
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("SEG FAULT");
                lcd.setCursor(0, 1);
                lcd.print("(CORE DUMPED)");

                vTaskDelete(NULL);
            } 
        }
        vTaskDelay(200/portTICK_PERIOD_MS); // 5 Hz frequency for task
    }
}

/**
 * @brief Task to control buzzer frequency based on time remaining.
 * 
 * Uses the ledc library to blink buzzer on and off at decreasing intervals as the time gets lower and lower. Intervals are 2 seconds, 1 second, 0.5 seconds, 0.25 seconds
 */
void BuzzerTimeTask(void *pvParameters) {
    // Counter to control buzzer frequency
    int counter = 0;

    while(1) {
        ledcWriteTone(0, 0);    // Buzzer is silenced

        // Win condition, Buzzer is silenced
        if (win) vTaskDelete(NULL);

        // Lose condition, Buzzer is constant
        if (lose) {
            ledcWriteTone(0, 1000);
            vTaskDelete(NULL);
        } 
        
        // Buzzer frequency based on time remaining
        if (timeCountDown > 25) {
            if (counter % 16 == 0) ledcWriteTone(0, 1000);           
        } else if (timeCountDown > 12) {
            if (counter % 8 == 0) ledcWriteTone(0, 1000);
        } else if (timeCountDown > 4) {
            if (counter % 4 == 0) ledcWriteTone(0, 1000);
        } else if (timeCountDown > 0) {
            if (counter % 2 == 0) ledcWriteTone(0, 1000);
        }
        
        counter++;
        vTaskDelay(128/portTICK_PERIOD_MS); // 8 Hz frequency for task
    }
}

/**
 * @brief Task to read joystick input and send to queue.
 * 
 * Reads analog voltage input from a joystick peripheral, and sends it to the AnalogX queue. Contains a debouncing mechanism to prevent unintentional inputs.
 */
void JoystickInputTask(void *pvParameters) {
    int analogX;        // used to store normalized analog input
    int send = -1;      // used to send direction to queue
    bool reset = true;  // used to debounce joystick input

    while(1) {
        if (joystickRunning) {
            analogX = (analogRead(joystick_x_pin) / 40.95); // Normalize analogRead to 0-100

            if (reset) {
                if (analogX > 50 && analogX < 80) {         // If we've recorded an input, we only allow another input once we detect motion to the center of the joystick first
                    reset = false;
                }
            } else {
                if (analogX > 80) {                         // Based on the voltage readings from analogRead in line 156
                    Serial.println("Right");
                    send = 1;                               // 1 (right) for the queue
                } else if (analogX < 50) {
                    Serial.println("Left");
                    send = 0;                               // 0 (left) for the queue
                }

                if (send != -1) {                           // if right or left was detected in this loop iteration
                    reset = true;                           // prompt debouncing mechanism for next iterations
                    xQueueSend(AnalogX, &send, 10);         //send input to the queue
                    send = -1;                              // reset the input value for next iterations
                }
            }
        }
        vTaskDelay(20/portTICK_PERIOD_MS);                  // 50 Hz frequency for task
    }
}

/**
 * @brief Task to generate random sequence of left and right directions and send to queue.
 *  
 * This task uses C's rand() function to generate binary values (either 1 or 0) and sends them to the freeRTOS queue before deleting itself
 */
void JoystickProducerTask(void *pvParameters) {
    // Randomly generate sequence of left and right (1/0) and send to queue
    for (int i = 0; i < 8; i++) {                   // Generate 8 random binary variables (0 or 1) to represent right or left
        int toQueue = rand() % 2;                   // Mod by 2 to get 0 or 1
        xQueueSend(DirectionsQueue, &toQueue, 10);  // Send 0 or 1 value to queue
        vTaskDelay(50/portTICK_PERIOD_MS);
    }

    joystickStart = true;                           // Boolean flag to start the joystick game after the queue is full/ready
    vTaskDelete(NULL);                              // Delete task as it's unused in the future
}

/**
 * @brief Task to handle joystick game sequence and user input.
 * 
 * Takes advantage of the freeRTOS queues, receiving values from both to construct a list of directions and a sequence of inputs to compare them to.
 * Fails the game if the user provides incorrect inputs.
 */
void JoystickGameTask(void *pvParameters) {
    // Array to store "left" and "right" strings for LCD display
    char* directions[2];   
    directions[0] = "left";
    directions[1] = "right";
    
    int order[8];           // Array to store correct sequence of left and right
    
    int numReceived = 0;    // Number of received direction values
    int receivedX;          // Placeholder for queue inputs
    bool correct = true;    // Keeps track of whether the user has entered the correct inputs so far
    int inputsReceived = 0; // Number of inputs received from the AnalogX queue
    int fromQueue;          // Placeholder for queue directions

    while(1) {
        if (joystickStart) {    // Start the joystick game sequence has been generated
            // Get correct sequence from queue
            while(xQueueReceive(DirectionsQueue, &fromQueue, 20) == pdPASS) { // Receive all values from directions queue
                order[numReceived] = fromQueue;
                numReceived++;  // Keep track of # of directions stored
            }

            // Print "Input Sequence ... Get Ready" prior to displaying sequence
            lcd.setCursor(10, 0);
            lcd.print("Input");
            lcd.setCursor(6, 1);
            lcd.print("Sequence:");
            vTaskDelay(800);
            lcd.clear();
            lcd.setCursor(6, 1);
            lcd.print("Get Ready");
            vTaskDelay(1000);
            lcd.clear();

            // Display sequence on LCD with appropriate delays between directions
            for(int i = 0; i < numReceived; i++) {
                lcd.setCursor(6, 1);
                lcd.print(directions[order[i]]);
                vTaskDelay(600/portTICK_PERIOD_MS);
                lcd.clear();

                vTaskDelay(500/portTICK_PERIOD_MS);
            }
            
            joystickRunning = true; // Begin joystick input detection
            joystickStart = false;  // End joystick prompting sequence

            vTaskDelay(100/portTICK_PERIOD_MS);
            
        } else if (joystickRunning) {
            if(xQueueReceive(AnalogX, &receivedX, 20) == pdPASS ) { // If an input is detected from queue
                inputsReceived++;
                
                if (receivedX != order[inputsReceived - 1]) { // In the case of an incorrect input, toggle the 'lose' flag and delete the current task
                    lose = true;
                    vTaskDelete(NULL);
                } else if (inputsReceived == numReceived) { // In the case of all correct inputs, toggle the 'joystickWin' flag and delete the current task
                    lcd.setCursor(0, 1);
                    lcd.print("Combo Found");
                    joystickWin = true;
                    vTaskDelete(NULL);
                }
            }
        } 
        vTaskDelay(31/portTICK_PERIOD_MS);  // 32 Hz frequency for task after initial sequence display
    }
}

/**
 * @brief Task to read input from Serial Monitor and compare to current RTC time.
 * 
 * Requires the user to input the time, in an HHMM (Hour Minute Minute) format to the serial monitor to complete the task. Uses the RTC module for real time measurement
 */
void SerialRTCInputTask(void *pvParameters) {
    while (1) {
        char inputBuffer[4];
        int hour;
        int minute;

    if (Serial.available() > 0) {
        // Get current hour and minute from RTC
        hour = RTC.getHours();
        minute = RTC.getMinutes();

        // Read 4 bytes/chars of input from Serial Monitor
        Serial.readBytes(inputBuffer, 4);

        // Clear Serial buffer
        while(Serial.available() > 0) Serial.read();
        
        // Time Checked for format of HHMM
        if (atoi(inputBuffer) == hour * 100 + minute) {
            RTCWin = true;
            if (!joystickWin) {
              lcd.setCursor(0, 1);
              lcd.print("RTC");
            }
            Serial.println("RTC Complete");
            vTaskDelete(NULL);
        } else {
          Serial.println("Try again");
        }
    }
    
    vTaskDelay(31/portTICK_PERIOD_MS);  // 32 Hz frequency for task
    }
}

void loop() {}
