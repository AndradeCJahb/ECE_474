// Filename:    Lab1Part3.ino
// Author:      Christopher Andrade (2221525)
// Date:        06/17/24
// Description: This file controls the asynchronous blinking of two off-board LEDs connected to an ESP32

// Version:     2.0
// Author:      Christopher Andrade (2221525)
// Date:        06/14/24
// Change(s):   Added functionality of lighting two seperate off-board LEDs at asynchronous intervals (one on, one off)

void setup() {
  pinMode (A7, OUTPUT); // initialize Port A7 as output
  pinMode (D2, OUTPUT); // initialize Port D2 as output
}

void loop() {
  digitalWrite(D2, HIGH); // Turn on LED associated with D2 port
  digitalWrite(A7, LOW);  // Turn off LED associated with A7 port
  delay(1000);
  digitalWrite(D2, LOW);  // Turn off LED associated with D2 port
  digitalWrite(A7, HIGH); // Turn on LED associated with A7 port
  delay(1000);
}
