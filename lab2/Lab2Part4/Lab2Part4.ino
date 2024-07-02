// Filename:    Lab2Step5.ino
// Author:      Christopher Andrade (2221525), Theo Favour (2169814)
// Date:        07/02/24
// Description: This file plays notes from an off-board passive buzzer based on the ambient light values from a photoresistor.

void setup() {
  // Initialize pin D2 as output (to passive buzzer), initialize A0 as analog input.
  pinMode(D2, OUTPUT);
  pinMode(A0, INPUT);

  ledcSetup(0, 1000, 12);   // PWM channel 0, arbitrary frequency of 1000 Hz, and 12 bits
  ledcAttachPin(D2, 0);     // Attach pin D2 (Passive Buzzer) to PWM channel 0
}

void loop() {
  if (analogRead(A0) < 1000) {  // Play notes if reading of the photoresistor is <1000 (Usually when photoresistor is completely covered)
    ledcWrite(0, 300);          // Note with passive buzzer duty of 300
    delay(1000);
    ledcWrite(0, 600);          // Note with passive buzzer duty of 600
    delay(1000);
    ledcWrite(0, 1200);         // Note with passive buzzer duty of 1200
    delay(1000);
  } else {
    ledcWrite(0, 0);            // Note with passive buzzer duty of 0 (off)
  }
  
}
