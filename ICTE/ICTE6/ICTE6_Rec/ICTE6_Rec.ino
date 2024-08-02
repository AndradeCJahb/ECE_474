// Filename:    ICTE6_Rec.ino
// Author:      Christopher Andrade (2221525), Theo Favour (2169814)
// Date:        07/10/2024
// Description: This file receives transmitted data bit by bit and prints the received data (2 bytes) to the serial monitor

int dataPin = D2;       // D2 pin for transimitting data
int ackPin  = D3;       // D3 pin for acknowledging that a bit has been received and processed
int intPin  = D4;       // D4 pin for interrupting receiver, indicating that a bit is being sent

int receivedValue = 0;  // initialize received Value to 0
int currentBit = 0 ;    // initialize current bit to 0

void setup() {
  pinMode(dataPin, INPUT);    // Receiver gets data as input
  pinMode(intPin, INPUT);     // Reciever is interrupted when bits are sent
  pinMode(ackPin, OUTPUT);    // Reciever outputs an acknowledgement of sent data
  digitalWrite(ackPin, LOW);  // Ensure Reciever does not acknowledge in advance
  
  attachInterrupt(digitalPinToInterrupt(intPin), intISR, HIGH);   // Attach interrupt to interrupt pin, when HIGH/1
  Serial.begin(9600);
}

void loop() { 
  // Print out recieved data once 16 bits (2 bytes) have been recieved and processed
  if(currentBit == 16) {
    Serial.println(receivedValue);
    delay(1000);

    // Reset value and current bit fields
    receivedValue = 0;
    currentBit = 0;
  }
}

// Description: Function that runs on interrupt, appends current data bit to recieved value
void intISR() { 
  // Read data from dataPin and mask it into next bits of recieved Value
  receivedValue |= digitalRead(dataPin) << currentBit;
  currentBit++;
  
  // Pulse Acknowledgement Pin
  digitalWrite(ackPin, HIGH); 
  digitalWrite(ackPin, LOW);
}
