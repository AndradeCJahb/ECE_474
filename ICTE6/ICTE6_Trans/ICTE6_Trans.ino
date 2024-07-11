// Filename:    ICTE6_Trans.ino
// Author:      Christopher Andrade (2221525), Theo Favour (2169814)
// Date:        07/10/2024
// Description: 

int dataPin = D2;
int ackPin  = D3; 
int intPin  = D4;  

int ackReceived = 0; // flag variable
int valueToSend = 21;

void setup() {
  pinMode(dataPin, OUTPUT);
  pinMode(intPin, OUTPUT);
  pinMode(ackPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(ackPin), ackISR, HIGH); 
  sendValue(valueToSend);
}

void loop() {

}

void ackISR() {
  ackReceived = 1;
}

void sendValue(int value) {
  for (int i = 0; i < 16; i++) {
    int bit = (value >> i) & 1;
    digitalWrite(dataPin, bit);// Ensure the bit is set before sending interrupt
    digitalWrite(intPin, HIGH);
    digitalWrite(intPin, LOW); // Pulse the interrupt pin
    while(!ackReceived);
    ackReceived = 0;
  }
}



