// Filename:    Lab2Part4.ino
// Author:      Christopher Andrade (2221525), Theo Favour (2169814)
// Date:        07/02/24
// Description: This file plays notes from an off-board passive buzzer based on the ambient light values from a photoresistor.

void setup() {
  // Initialize pin D2 as output (to passive buzzer), initialize A0 as input.
  pinMode(D2, OUTPUT);
  pinMode(A0, INPUT);

  ledcSetup(0, 1000, 12); // PWM channel 0, arbitrary frequency of 1000 Hz, and 12 bits
  ledcAttachPin(D2, 0);   // Attach pin D2 (Passive Buzzer) to PWM channel 0
}

void loop() {
  if (analogRead(A0) > 2000) {  // Play notes if reading of the A0 port is >2000
    ledcWrite(0, 500);          // Note with passive buzzer duty of 500
    delay(1000);
    ledcWrite(0, 1100);         // Note with passive buzzer duty of 1100
    delay(1000);
    ledcWrite(0, 2600);         // Note with passive buzzer duty of 2600
    delay(1000);
    ledcWrite(0, 3600);         // Note with passive buzzer duty of 3600
    delay(1000);
  } else {
    ledcWrite(0, 0);            // Note with passive buzzer duty of 0 (off)
  }
}
