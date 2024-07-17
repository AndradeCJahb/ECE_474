// Filename:    Lab3Part2.ino
// Author:      Christopher Andrade (2221525), Theo Favour (2169814)
// Date:        07/15/24
// Description: 

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

struct TCB {
 void (*taskFunction)();    // Pointer to the task function
 bool isRunning;            // State of the task
 bool isDone;               // Whether the task is completed or not
 int pid;                   // Unique process ID
 int priority;              // The priority level of the task
};

LiquidCrystal_I2C lcd(0x27, 16, 2); // Initialize the LCD

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

  //struct TCB LED, Count10, Buzz, Alpha;
  struct TCB LED      = {blinkLED   , false, false, 1, 1};
  struct TCB Count10  = {counter    , false, false, 2, 2};
  struct TCB Buzz     = {playBuzzer , false, false, 3, 3};
  struct TCB Alpha    = {printAlpha , false, false, 4, 4};

  struct TCB tasks[4] = {LED, Count10, Buzz, Alpha};

  delay(20);
}

void loop() {
  // Check if all tasks done
  if(tasks[0].isDone && tasks[1].isDone && tasks[2].isDone && tasks[3].isDone) {
    for(int i = 0; i < 4; i++) {
      tasks[i].isDone = false;
      tasks[i].priority = tasks[i].priority % 4 + 1;
    }
  }

  for(int = 0; i < 4; i++) {
    
  }




  delay(4000);
}

void blinkLED() {
  LED.isRunning = true;

  int count = 0;
  long startTime = millis();

  while(count < 16) {
    if(millis()-startTime > 1000) {
      digitalWrite(D2, !digitalRead(D2));
      startTime = millis();
      count++;
    }
  }

  LED.isRunning = false;
  LED.isDone    = true;
  Serial.print("LED Blinker: ");
  Serial.println(LED.priority);
}

void counter() {
  Count10.isRunning = true;

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

  // Transmission of byte array containing displaying of 1-10 on LCD
  Wire.beginTransmission(0x27);
  Wire.write(countBytes, 44);
  Wire.endTransmission();
  delay(2);

  Count10.isRunning = false;
  Count10.isDone    = true;
  Serial.print("Counter: ");
  Serial.println(Count10.priority);
}

void playBuzzer() {
  Buzz.isRunning = true;
  
  int count = 0;
  long startTime = millis();

  while(count < 10) {
    if(millis()-startTime > 500) {
      ledcWrite(0, (count+1)*400);
      startTime = millis();
      count++;
    }
  }

  Buzz.isRunning = false;
  Buzz.isDone    = true;
  Serial.print("Music Player: ");
  Serial.println(Buzz.priority);
}

void printAlpha() {
  Alpha.isRunning = true;

  Serial.print("\n");
  for(int i = 0; i < 26; i++) {
    Serial.print((char)(i+65));
    if(i != 25) {
      Serial.print(',');
    }
  }

  Alpha.isRunning = false;
  Alpha.isDone    = true;
  Serial.print("Alphabet Printer: ");
  Serial.println(Alpha.priority);
}