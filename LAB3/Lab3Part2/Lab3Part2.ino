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

#define LEDPin D2
#define BuzzPin D3

LiquidCrystal_I2C lcd(0x27, 16, 2); // Initialize the LCD

void setup() {
  Serial.begin(115200);

  // Initialize LCD and I2C communication
  Wire.begin();
  lcd.init();

  // Initialize LED pin
  pinMode(LEDPin, OUTPUT);
  digitalWrite(LEDPin, LOW);

  // Initialize Buzzer pin and corresponding LEDC PWM module
  pinMode(BuzzPin, OUTPUT);
  ledcSetup(0, 1000, 12);     // PWM channel 0, arbitrary frequency of 1000 Hz, and 12 bits
  ledcAttachPin(BuzzPin, 0);
  ledcWrite(0, 0);

  delay(2);
}

void loop() {
  struct TCB LED, count10, Buzz, Alpha;

  
}

void blinkLED() {
  int count = 0;
  long startTime = millis();

  while(count < 16) {
    if(millis()-startTime % 1000 == 0) {
      digitalWrite(LEDPin, ~digitalRead(LEDPin));
      count++;
    }
  }
}

void counter() {
  uint8_t countBytes[40];

  for(int i = 0; i < 10; i++) {
    uint8_t currChar = i+49;
    countBytes[4*i]    = 0x0D | ((currChar) & 0xF0);        // Mask upper bits of char with HIGH enable bit instruction
    countBytes[4*i+1]  = 0x09 | ((currChar) & 0xF0);        // Mask upper bits of char with LOW enable bit instruction
    countBytes[4*i+2]  = 0x0D | (currChar << 4);            // Shift and mask lower bits of char with HIGH enable bit instruction
    countBytes[4*i+3]  = 0x09 | (currChar << 4);              // Shift and mask lower bits of char with LOW enable bit instruction
  }

  Wire.beginTransmission(0x27);
  Wire.write(countBytes, 40);
  Wire.endTransmission();
  delay(2);
}

void playBuzzer() {
  int count = 0;
  while(count < 10) {
    if(millis()-startTime % 500 == 0) {
      ledcWrite(0, (count+1)*400);
      count++;
    }
  }
}

void printAlpha() {
  for(int i = 0; i < 26; i++) {
    Serial.print((char)(i+65));
    if(i != 25) {
      Serial.print(',');
    }
  }
}