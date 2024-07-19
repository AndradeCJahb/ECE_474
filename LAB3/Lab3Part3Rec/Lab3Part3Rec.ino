// Filename:    Lab3Part3Rec.ino
// Author:      Christopher Andrade (2221525), Theo Favour (2169814)
// Date:        07/15/24
// Description: This file counts upwards every second on an off-board LCD, and displays received wireless messages from another ESP32 on the same LCD.

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <esp_now.h>
#include <WiFi.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Initialize the LCD
hw_timer_t* timer = NULL;  // Initialize various timer and boolean variables for the interrupt ISRs
volatile bool msg = false;
volatile int counter = 0;
volatile bool update = false;
uint8_t buffer[16]; // Buffer to store data to be sent to LCD

int inputLength;

// Timer interrupt service routine to increment number on the LCD
void IRAM_ATTR timerInterrupt() {
  counter += 1;
  update = true;
}

// Interrupt service routine to display message on LCD when received
void IRAM_ATTR dataReceived(const uint8_t * mac, const uint8_t *incomingData, int len) {
  msg = true;
  update = true;
  for(int i = 0; i < len && i < 16; i++) { //copy the received bytes into the global buffer
    buffer[i] = incomingData[i];
  }
  inputLength = len;
}

void setup() {  
  // Initialize LCD, WIFI, and I2C communication
  Wire.begin();
  lcd.init();
  WiFi.mode(WIFI_STA);
  delay(2);

  Serial.begin(115200);
  while(!Serial){}

  timer = timerBegin(0, 80, true);  // Timer 0, prescaler 80, count up
  timerAttachInterrupt(timer, &timerInterrupt, true);  // Attach timerInterrupt function to the timer, triggered on the rising edge (true)
  // Set alarm to trigger interrupt every second, repeating (true)
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);  // Enable the alarm

  // Initializes ESP-NOW and check if it was successful; if not, exit the setup function
  if (esp_now_init() != ESP_OK) return;
 
  // Registers the callback function 'dataReceived' to be called when data is received via ESP-NOW
  esp_now_register_recv_cb(dataReceived);
}

void loop() {
  if(update) { //only update the LCD display when one of the interrupts is triggered

    if(!msg) { //if there's not a new message, increment the count variable
      
      inputLength = 0;
      int x = counter; //placeholder variable to determine the number of digits in the current display number
      int i = 0; //variable to ensure we don't go past the limits of the LCD display character count

      while(x >= 1 && i < 16) { // find the input length by repeatedly dividing the value of the counter by 10
        inputLength += 1;
        buffer[i] = (uint8_t) ((x % 10) + 48); // set the buffer to be the correct number, but backwards by taking each digit as x is divided by 10
        x = x / 10;
        i += 1;
      }

      int j = 0; //reverse the buffer to get the correct number using a temporary variable to swap opposite ends of the array
      while (j < i / 2) { 
        uint8_t temp = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = temp;
        j += 1;
      }
    }

    uint8_t commBytes[64] = { 0 };

    // Create arrays with instructions for clearing the LCD and setting the cursor to the first line
    uint8_t clear[] = { 0x0C, 0x08, 0x1C, 0x18 };
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
    for (int i = 0; i < inputLength; i++) {
      commBytes[4 * i]      = 0x0D | (buffer[i] & 0xF0);             // Mask upper bits of char with HIGH enable bit instruction
      commBytes[4 * i + 1]  = 0x09 | (buffer[i] & 0xF0);         // Mask upper bits of char with LOW enable bit instruction
      commBytes[4 * i + 2]  = 0x0D | (buffer[i] << 4);  // Shift and mask lower bits of char with HIGH enable bit instruction
      commBytes[4 * i + 3]  = 0x09 | (buffer[i] << 4);  // Shift and mask lower bits of char with LOW enable bit instruction
    }

    // Transmit array of instructions corresponding to writing chars to LCD
    Wire.beginTransmission(0x27);
    Wire.write(commBytes, inputLength * 4);
    Wire.endTransmission();
    delay(2);
    update = false;

    //if there has been a received message this update, delay for 2 seconds to show the message before continuing to count up
    if (msg) {
      delay(2000);
      msg = false;
    }
  }
}

