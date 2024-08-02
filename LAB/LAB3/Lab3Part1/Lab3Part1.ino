// Filename:    Lab3Part1.ino
// Author:      Christopher Andrade (2221525), Theo Favour (2169814)
// Date:        07/15/24
// Description: This file displays the first 16 characters typed into the Arduino Serial Monitor on an off-board LCD display.

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Initialize the LCD

void setup() {
  Serial.begin(115200);
  
  // Initialize LCD and I2C communication
  Wire.begin();
  lcd.init();
  delay(2);
}

void loop() {
  // Create buffers to hold up to 16 entered bits and 64 1-byte instructions
  uint8_t buffer[16] = {0};
  uint8_t commBytes[64] = {0};

  // Wait until Serial input
  while(Serial.available() == 0){
  }

  // Place entered string into buffer array and determine length of entered string
  int inputLength = Serial.readBytesUntil('\r', buffer, 16) -1;

  // Clear remaining bytes in Serial Monitor
  while(Serial.available() > 0) {
    char t = Serial.read();
    t = t ;  // Avoid compiler warnings about how t is "unused"
  }
  
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
  
  // Create instructions concerning LCD display according to entered string
  for(int i = 0; i < inputLength; i++) {
    commBytes[4*i]    = 0x0D | (buffer[i] & 0xF0);        // Mask upper bits of char with HIGH enable bit instruction
    commBytes[4*i+1]  = 0x09 | (buffer[i] & 0xF0);        // Mask upper bits of char with LOW enable bit instruction
    commBytes[4*i+2]  = 0x0D | ((buffer[i] & 0x0F) << 4); // Shift and mask lower bits of char with HIGH enable bit instruction
    commBytes[4*i+3]  = 0x09 | ((buffer[i] & 0x0F) << 4); // Shift and mask lower bits of char with LOW enable bit instruction
  }

  // Transmit array of instructions corresponding to writing chars to LCD
  Wire.beginTransmission(0x27);
  Wire.write(commBytes, inputLength*4);
  Wire.endTransmission();
  delay(2); 
}
