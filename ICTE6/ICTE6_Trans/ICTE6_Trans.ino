// Filename:    ICTE6_Trans.ino
// Author:      Christopher Andrade (2221525), Theo Favour (2169814)
// Date:        07/10/2024
// Description: Transmits an integer value bit by bit to another ESP32 connected with wires

int dataPin = D2;
int ackPin  = D3; 
int intPin  = D4;  

volatile int ackReceived = 0; // flag variable
int valueToSend = 21;

void setup() {
  pinMode(dataPin, OUTPUT); // set transmitting pins to output, initialiize to off
  pinMode(intPin, OUTPUT);
  digitalWrite(intPin, LOW);
  pinMode(ackPin, INPUT); // set acknowledgement pin to input from other ESP32
  attachInterrupt(digitalPinToInterrupt(ackPin), ackISR, HIGH); // interrupt when acknowledgement pin set to HIGH
  sendValue(valueToSend);
  
}

void loop() {

}

// Description: function runs on interrupt, notes acknowledgement from other ESP32
void ackISR() { 
  ackReceived = 1;
}

void sendValue(int value) { // transmits the specified value to the other ESP32
  for (int i = 0; i < 16; i++) {
    int bit = (value >> i) & 1; // send each of the 16 bits of the desired number at a time
    digitalWrite(dataPin, bit);// Ensure the bit is set before sending interrupt
    digitalWrite(intPin, HIGH);
    digitalWrite(intPin, LOW); // Pulse the interrupt pin
    while(!ackReceived) { // awaits the acknowledgement interrupt

    }
    ackReceived = 0; // resets the acknowledgement boolean variable
    delay(100);
  }
}