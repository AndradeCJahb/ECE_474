// Filename:    ICTE6_Rec.ino
// Author:      Christopher Andrade (2221525), Theo Favour (2169814)
// Date:        07/10/2024
// Description: 

int dataPin = D2;
int ackPin  = D3; 
int intPin  = D4;  

int ackReceived = 0;  // flag variable
int valueToSend = 21;

int dataReady = 0; 
int receivedValue; 
int currentBit = 0 ; // global variables

void setup() {
  pinMode(dataPin, INPUT);
  pinMode(intPin, INPUT);
  pinMode(ackPin, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(intPin), intISR, HIGH);
  Serial.begin(9600);
}

void loop() { 
  if(currentBit == 16) {
    Serial.println(recievedValue);
    delay(1000);
  }
}

void intISR() { 
  receivedValue |= digitalRead(dataPin) << currentBit  ; //use bit masking 
  currentBit++;
  digitalWrite(ackPin, HIGH); 
  digitalWrite(ackPin, LOW); 
}
