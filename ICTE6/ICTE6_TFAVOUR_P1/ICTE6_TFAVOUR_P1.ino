/*filename: ICTE6_TFAVOUR_P1.ino
Author: Theo Favour
Date: 7/11/24
Description: The LED turns on when the tactile switch is pressed, and turned off when it's released */

int power;

//function implementations
void setup() {
  // put your setup code here, to run once:
  int power = 0;
  pinMode(D2, OUTPUT); //D2 is distinct led
  pinMode(D3, INPUT_PULLUP); //D3 is tactile switch
  attachInterrupt(digitalPinToInterrupt(D3), onoff, CHANGE);  //interrupts the code when D3 (switch) switches from off to on or vice versa
  digitalWrite(D2, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(D2, power);
}

void onoff() { //toggles the power variable which is constantly setting the value of the LED to on or off
  if (power == 0) {
    power = 1;
  } else {
    power = 0;
  }
}
