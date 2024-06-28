// Filename:    Lab1Part4.ino
// Author:      Christopher Andrade (2221525)
// Date:        06/17/24
// Description: This file utilizes an off-board tactile switch to control the status of an off-board LED
//              (If the button is pressed the LED lights, LED is off otherwise)

void setup() {
  pinMode (D3, INPUT_PULLUP); // initialize Port D3 as a "PULLED UP" input
  pinMode (D2, OUTPUT); // initialize Port D2 as output
}

void loop() {
  if (!digitalRead(D3)) { // Utilizing INPUT_PULLUP therefore reads HIGH when button is not pressed, must NOT (!) the read. 
    digitalWrite(D2, HIGH); // Light LED when button is pushed
  } else {
    digitalWrite(D2, LOW);  // LED is off otherwise (button not pushed)
  }
}
