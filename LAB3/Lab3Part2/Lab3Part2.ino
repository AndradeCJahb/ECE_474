// Filename:    Lab3Part2.ino
// Author:      Christopher Andrade (2221525), Theo Favour (2169814)
// Date:        07/15/24
// Description: This file contains 4 different tasks of changing priority which are then run according
//              to their current priority by use of a TCB.

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Function prototypes
void blinkLED();
void counter();
void playBuzzer();
void printAlpha();

// TCB struct containing, task/function pointer, priority and variables concerning task status
struct TCB {
 void (*taskFunction)();    // Pointer to the task function
 bool isRunning;            // State of the task
 bool isDone;               // Whether the task is completed or not
 int priority;              // The priority level of the task
};

LiquidCrystal_I2C lcd(0x27, 16, 2); // Initialize the LCD

// Creation of structs corresponding to a unique task with unique priority
struct TCB LED      = {blinkLED   , false, false, 4};
struct TCB Count10  = {counter    , false, false, 3};
struct TCB Buzz     = {playBuzzer , false, false, 2};
struct TCB Alpha    = {printAlpha , false, false, 1};

// Creation and placement of tasks into larger TCB array
struct TCB tasks[4] = {LED, Count10, Buzz, Alpha};

void setup() {
  Serial.begin(115200);

  // Initialize LCD and I2C communication
  Wire.begin();
  lcd.init();

  // Initialize LED pin
  pinMode(D2, OUTPUT);
  digitalWrite(D2, LOW);

  // Initialize Buzzer pin and corresponding LEDC PWM module
  pinMode(D3, OUTPUT);
  ledcSetup(0, 1000, 12);     // PWM channel 0, arbitrary frequency of 1000 Hz, and 12 bits
  ledcAttachPin(D3, 0);
  ledcWrite(0, 0);

  delay(200);
}

void loop() {
  // Check if all tasks done
  if(tasks[0].isDone && tasks[1].isDone && tasks[2].isDone && tasks[3].isDone) {
    // Reset status and change priority of all tasks/functions
    for(int i = 0; i < 4; i++) {
      tasks[i].isDone = false;
      tasks[i].isRunning = false;
      tasks[i].priority = tasks[i].priority % 4 + 1;
    }
  }

  // Find next task to run based on highest priority remaining
  int nextTaskIndex = -1;     // Initialize taskindex to 1, corresponds to no task
  int nextTaskPriority = -1;  // Priority should be overwritten immediately if a valid task is found
  for(int i = 0; i < 4; i++) {
    // Next task determined if it is not running, done and if it has a greater priority than current highest priority
    if(!tasks[i].isDone && !tasks[i].isRunning && tasks[i].priority > nextTaskPriority) {
      nextTaskIndex = i;
      nextTaskPriority = tasks[i].priority;
    }
  }

  // if a valid task to run has been found
  if (nextTaskIndex != -1) {
    tasks[nextTaskIndex].isRunning = true;
    tasks[nextTaskIndex].taskFunction();            // Call function/task
    tasks[nextTaskIndex].isDone = true;
    tasks[nextTaskIndex].isRunning = false;
    Serial.println(tasks[nextTaskIndex].priority);  // Print priority of task after completion
  }
}

// Description: Blink an off-board LED on/off at 1000ms intervals (8 times each)
void blinkLED() {
  int count = 0;
  long startTime = millis();

  while(count < 16) {
    if(millis()-startTime > 1000) {
      digitalWrite(D2, !digitalRead(D2));
      startTime = millis();
      count++;
    }
  }

  Serial.print("LED Blinker: ");
}

void counter() {
  // Create arrays with instructions for clearing the LCD and setting the cursor to the first line
  uint8_t clear[]  = { 0x0C, 0x08, 0x1C, 0x18 };
  uint8_t cursor[] = { 0x8C, 0x88, 0x0C, 0x08 };

  // Transmit array of instructions corresponding to clearing the LCD
  Wire.beginTransmission(0x27);
  Wire.write(clear, 4);
  Wire.endTransmission();
  delay(2);

  // Transmit array of instructions corresponding to setting the cursor to the first line
  Wire.beginTransmission(0x27);
  Wire.write(cursor, 4);
  Wire.endTransmission();
  delay(2);

  // Creation of array to contain bytes corresponding to counting 1-10 on LCD
  uint8_t countBytes[44];

  // Creation of data bytes corresponding to counting 1-9 on LCD 
  for(int i = 0; i < 9; i++) {
    uint8_t currChar = i+49;
    countBytes[4*i]    = 0x0D | (currChar & 0xF0);          // Mask upper bits of char with HIGH enable bit instruction
    countBytes[4*i+1]  = 0x09 | (currChar & 0xF0);          // Mask upper bits of char with LOW enable bit instruction
    countBytes[4*i+2]  = 0x0D | (currChar << 4);            // Shift and mask lower bits of char with HIGH enable bit instruction
    countBytes[4*i+3]  = 0x09 | (currChar << 4);            // Shift and mask lower bits of char with LOW enable bit instruction
  }

  // "Hard coding" of bytes representing printing of 1 and 0 (10) into byte array sent to LCD
  countBytes[36] = 0x0D | (uint8_t)(49 & 0xF0);
  countBytes[37] = 0x09 | (uint8_t)(49 & 0xF0);
  countBytes[38] = 0x0D | (uint8_t)(49 << 4);
  countBytes[39] = 0x09 | (uint8_t)(49 << 4);

  countBytes[40] = 0x0D | (uint8_t)(48 & 0xF0);
  countBytes[41] = 0x09 | (uint8_t)(48 & 0xF0);
  countBytes[42] = 0x0D | (uint8_t)(48 << 4);
  countBytes[43] = 0x09 | (uint8_t)(48 << 4);

  
  uint8_t* countBytesPtr;
  int count = 0;
  long startTime = millis();

  while(count < 10) {
    if(millis()-startTime >= 500) {
      countBytesPtr = &countBytes[count*4];

      Wire.beginTransmission(0x27);
      Wire.write(cursor, 4);
      Wire.endTransmission();
      delay(2);
      

      Wire.beginTransmission(0x27);
      Wire.write(countBytesPtr, 4);
      Wire.endTransmission();
      delay(2);
      startTime = millis();
      count++;
    }
  }

  countBytesPtr = &countBytes[40];

  Wire.beginTransmission(0x27);
  Wire.write(countBytesPtr, 4);
  Wire.endTransmission();
  delay(2);

  Serial.print("Counter: ");
}

void playBuzzer() {
  int count = 0;
  long startTime = millis();

  // Play 10 different notes of ascending duty rate every 500 ms
  while(count < 10) {
    if(millis()-startTime >= 500) {
      ledcWrite(0, (count+1)*400);
      startTime = millis();
      count++;
    }
  }

  // Turn off passive Buzzer
  ledcWrite(0, 0);
  Serial.print("Music Player: ");
}

// Description: Prints alphabet with characters seperated by commas to Serial Monitor
void printAlpha() {
  // Loop through numbers 0-25 and normalize (+65) to obtain corresponding ASCII characters
  for(int i = 0; i < 26; i++) {
    Serial.print((char)(i+65));

    // Do not print "," if last Character (Z)
    if(i != 25) {
      Serial.print(',');
    }
  }
  Serial.println();

  Serial.print("Alphabet Printer: ");
}