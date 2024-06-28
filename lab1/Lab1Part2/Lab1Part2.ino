// Filename:    Lab1Part2.ino
// Author:      Christopher Andrade (2221525)
// Date:        06/17/24
// Description: This file contains the blinking of the built-in LED on an ESP32
//              at one second intervals (one second on, one second off)

void setup() {
  pinMode (13, OUTPUT); // initialize the built-in LED Pin (13) as output
}

void loop() {
  digitalWrite(13, HIGH); // Turn on Built-in LED
  delay(1000);
  digitalWrite(13, LOW);  // Turn off Built-in LED
  delay(1000);
}
