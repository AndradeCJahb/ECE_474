// Filename:    ICTE6_Lab1.ino
// Author:      Christopher Andrade (2221525), Theo Favour (2169814)
// Date:        07/10/2024
// Description: This file utilizes an off-board tactile switch and interrupts to control the status of an off-board LED
//              (If the button is pressed the LED lights, LED is off otherwise)

int state = 0;                                                    // State which is written to the D2 LED pin

void setup() {
  pinMode (D3, INPUT_PULLUP);                                     // initialize Port D3 as a "PULLED UP" input
  pinMode (D2, OUTPUT);                                           // initialize Port D2 as output
  attachInterrupt(digitalPinToInterrupt(D3), toggle, CHANGE);     // Attach interrupt to changes in D3 pin
  digitalWrite(D2, 0);                                            // Ensure D2 pin starts LOW/0
}

void loop() {
  digitalWrite(D2, state);                                        // constantly write the state variable to LED (1 or 0, HIGH or LOW)
}

// Description: Function that toggles state variable on interrupt (change in D3)
void toggle() {
  state = !state;                                                 // Toggle state variable
}