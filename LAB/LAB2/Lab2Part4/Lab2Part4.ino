/**
 * @file Lab2Part4.ino
 * @brief Plays notes from an off-board passive buzzer based on the ambient light values from a photoresistor
 *
 * This file contains code to play notes using a passive buzzer connected to pin D2 based on the ambient light values read from a photoresistor connected to pin A0.
 * 
 * @date 07/02/24
 * @authors Christopher Andrade, Theo Favour
 */

long startTime;    /**< First time when light above threshold is recorded */
int count;         /**< Number of pitches played */
bool check;        /**< True if currently playing a cycle, false otherwise */

/**
 * @brief Setup function to configure pins and PWM
 *
 * This function initializes pin D2 as an output (to passive buzzer) and pin A0 as an input (from photoresistor). It also sets up PWM on channel 0.
 */
void setup() {
  pinMode(D2, OUTPUT);  // Initialize pin D2 as output (to passive buzzer)
  pinMode(A0, INPUT);   // Initialize pin A0 as input (from photoresistor)
  
  count = 0;
  check = false;

  // PWM setup: channel 0, frequency of 1000 Hz, resolution of 12 bits
  ledcSetup(0, 1000, 12);
  // Attach pin D2 (passive buzzer) to PWM channel 0
  ledcAttachPin(D2, 0);
}

/**
 * @brief Loop function to play notes based on ambient light level
 *
 * This function plays notes using the passive buzzer if the ambient light level read from the photoresistor exceeds a threshold. The notes are played in a cycle, and the cycle is repeated if the light level remains above the threshold.
 */
void loop() {
  // Play notes if reading from A0 port is >1000, or if already playing a note cycle
  if ((analogRead(A0) > 1000) || check) {
    if (!check) {  // If starting a new cycle
      check = true;
      startTime = millis();
    }
    if (millis() - startTime >= (count * 1000)) {  // Every 1000 milliseconds, increment count and set the duty cycle for a new note
      count += 1;
      if (count == 5) {  // If 4 notes have been played, reset check and count for a new cycle
        check = false;
        count = 0; 
      } else {
        ledcWrite(0, 1600 + (count * 600));  // Set new duty cycle for a new note
      }
    }  
  }
