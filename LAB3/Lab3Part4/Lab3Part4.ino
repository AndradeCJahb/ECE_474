// Filename:    Lab3Part2.ino
// Author:      Christopher Andrade (2221525), Theo Favour (2169814)
// Date:        07/15/24
// Description: 

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Function prototypes
void blinkLED();
void counter();
void playBuzzer();
void printAlpha();
void IRAM_ATTR buttonInterrupt();

// global variables of current task, and if an interuppt has been detected
int nextTaskIndex = -1;
bool interrupt;

struct TCB {
 void (*taskFunction)();    // Pointer to the task function
 bool isRunning;            // State of the task
 bool isDone;               // Whether the task is completed or not
 int pid;                   // Unique process ID
 int priority;              // The priority level of the task
};

LiquidCrystal_I2C lcd(0x27, 16, 2); // Initialize the LCD

// Creation of TCB structs for each task
struct TCB LED      = {blinkLED   , false, false, 1, 4};
struct TCB Count10  = {counter    , false, false, 2, 3};
struct TCB Buzz     = {playBuzzer , false, false, 3, 2};

// Insertion of TCB structs into TCB array
struct TCB tasks[3] = {Buzz, Count10, LED};

void setup() {
  Serial.begin(115200);

  // Initialize LCD and I2C communication
  Wire.begin();
  lcd.init();

  // Initialize D2, LED pin
  pinMode(D2, OUTPUT);
  digitalWrite(D2, LOW);

  // Initialize D3, buzzer pin
  pinMode(D3, OUTPUT);

  // Initialize D5, Button/Interrupt pin
  pinMode(D5, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(D5), &buttonInterrupt, FALLING); // attach an interrupt to call buttonInterrupt() on button press

  delay(200);
}

void IRAM_ATTR buttonInterrupt() {
  interrupt = true;
}


void loop() {
  // If all tasks done, set all tasks not done and not running
  if(tasks[0].isDone && tasks[1].isDone && tasks[2].isDone) {
    for(int i = 0; i < 3; i++) {
      tasks[i].isDone = false;
      tasks[i].isRunning = false;
    }
  }

  // Obtain next task to run based on isDone and isRunning status
  nextTaskIndex = -1;
  for(int i = 0; i < 3; i++) {
    if(!tasks[i].isRunning && !tasks[i].isDone) {
      nextTaskIndex = i;
      break;
    }
  }

  // If a task has been found to run, run it and set appropriate flags within TCB struct
  if (nextTaskIndex != -1) {
    tasks[nextTaskIndex].isRunning = true;
    tasks[nextTaskIndex].taskFunction();
    tasks[nextTaskIndex].isDone = true;
    tasks[nextTaskIndex].isRunning = false;
  }
}

// Description: Blink an off-board LED on/off at 500 ms intervals (10 times each)
void blinkLED() {
  int count = 0;
  long startTime = millis();

  // Loop which toggles the LED pin 20 times
  while(count < 20) {
    // Check for interrupt flag, return to loop() and toggle of interrupt flag
    if(interrupt) {
      interrupt = false;
      digitalWrite(D2, LOW);
      return;
    }

    // if 500 ms have passed, toggle LED pin (D2), increment count, reset start time
    if(millis()-startTime >= 500) {
      digitalWrite(D2, !digitalRead(D2));
      startTime = millis();
      count++;
    }
  }

  Serial.println("LED Blinker");
}

// Description: Counts from 1-10 at 1 second intervals on the off-board LCD
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

  uint8_t* countBytesPtr;     // Pointer to specific index within created countBytes array
  int count = 0;
  long startTime = millis();

  // Loop 10 times to count up on LED
  while(count < 10) {
    // Check for interrupt flag, return to loop() and toggle of interrupt flag
    if(interrupt) {
      interrupt = false;
      return;
    }
    
    // if 1000 ms have passed transmit new data (number from counter) to LCD, increment count and "reset" start time
    if(millis()-startTime >= 1000) {
      countBytesPtr = &countBytes[count*4];   // pointer depends on current count

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

  // Transmit '0' in 10 at end of count.
  countBytesPtr = &countBytes[40];

  Wire.beginTransmission(0x27);
  Wire.write(countBytesPtr, 4);
  Wire.endTransmission();
  delay(2);

  Serial.println("Counter");
}

// Description: Plays a 10 note melody using the passive buzzer
void playBuzzer() {
  int count = 0;
  long startTime = millis();

  // notes array contains: C, Eb, f, f#, F, Eb, C, Bb, D, C
  int notes[10] = { 523, 622, 698, 740, 698, 622, 523, 466, 587, 523};
  int duration = 500;

  // Loop through entire notes array
  while(count < 10) {
    // Check for interrupt flag, return to loop() and toggle of interrupt flag
    if(interrupt) {
      interrupt = false;
      noTone(D3);
      return;
    }

    // Plays note for a set duration depending on it the current count (index) within the notes array
    if(millis()-startTime > duration) {
      if (count == 7 || count == 8) {
        duration = 150;
      } else if (count == 6) {
        duration = 900;
      } else if (count == 9) {
        duration = 1800;
      } else {
        duration = 300;
      }
      tone(D3, notes[count]);
      startTime = millis();
      count++;
    }
  }

  noTone(D3); // turn off passive buzzer
  Serial.println("Music Player");
}