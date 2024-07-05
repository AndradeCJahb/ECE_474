// Filename:    Lab2Part4.ino
// Author:      Christopher Andrade (2221525), Theo Favour (2169814)
// Date:        07/02/24
// Description: This file plays notes from an off-board passive buzzer based on the ambient light values from a photoresistor.

long startTime;                                       // global variables: startTime: first time when light above threshold recorded, count: number of pitches played, check: true if currently playing a cycle, false otherwise
int count;
bool check;

void setup() {
  pinMode(D2, OUTPUT);                                // initialize pin D2 as output (to passive buzzer), initialize A0 as input.
  pinMode(A0, INPUT); 
  
  count = 0;
  check = false;

  ledcSetup(0, 1000, 12);                             // PWM channel 0, arbitrary frequency of 1000 Hz, and 12 bits
  ledcAttachPin(D2, 0);                               // attach pin D2 (Passive Buzzer) to PWM channel 0
}

void loop() {
  if ((analogRead(A0) > 1000) || check) {             // play notes if reading of the A0 port is >1000, or if already playing a note cycle
    if (check == false) {                             // if this is true, we are starting a cycle and check is false, set check to true & log start time
      check = true;
      startTime = millis();
    }
    if (millis() - startTime >= (count * 1000)) {     // every 1000 milliseconds, increment count by one and set the duty cycle for a new note
      count += 1;
      if (count == 5) {                               // if we've played 4 notes already, set check to false to re-check the ambient light level on the next loop, reset count to replay the note cycle
        check = false;
        count = 0; 
      } else {
        ledcWrite(0, 1600 + (count * 600));           // set new duty cycle for a new note
      }
    }  
  } else if (!check) {                                // if light level is below threshold AND note cycle is finished or not playing
    ledcWrite(0, 0);                                  // note with passive buzzer duty of 0 (off)
  }
}