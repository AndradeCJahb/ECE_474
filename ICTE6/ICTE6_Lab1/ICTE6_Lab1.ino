// Filename:    ICTE6_Lab1.ino
// Author:      Christopher Andrade (2221525), Theo Favour (2169814)
// Date:        07/10/2024
// Description: This file utilizes an off-board tactile switch and interrupts to control the status of an off-board LED
//              (If the button is pressed the LED lights, LED is off otherwise)

int state = 0;

void setup() {
  pinMode (D3, INPUT_PULLUP);                                     // initialize Port D3 as a "PULLED UP" input
  pinMode (D2, OUTPUT);                                           // initialize Port D2 as output
  attachInterrupt(digitalPinToInterrupt(D3), toggle, CHANGE);
  digitalWrite(D2, 0);
}

void loop() {
  digitalWrite(D2, state);
}

void toggle() {
  state = !state;
}