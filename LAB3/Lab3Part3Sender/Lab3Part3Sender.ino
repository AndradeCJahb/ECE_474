/*filename: Lab3Part3Sender.ino
Author: Theo Favour, Christopher Andrade
Date: 6/17/24
Description: Sends a message to be received on the serial monitor of the other ESP32 */

// MAC Address: 74:4D:BD:7D:40:18

#include <esp_now.h>
#include <WiFi.h>

// Replace with receiver's MAC address
uint8_t broadcastAddress[] = {0x74, 0x4D, 0xBD, 0x7D, 0x40, 0x18}; 
volatile bool buttonPressed = false;

void IRAM_ATTR buttonInterrupt() {
  buttonPressed = true;
}

void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
 // Callback function called when data is sent
 // Check if the delivery was successful and print the status
 Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Failed");
}

void setup() {
  Serial.begin(9600); //initialize and wait for serial so we can print results
  while (!Serial) {}

  WiFi.mode(WIFI_STA);
  pinMode(D2, INPUT_PULLUP); //initialize wifi mode, set pin D2 (switch) to be input_pullup which means LOW when pressed
  attachInterrupt(digitalPinToInterrupt(D2), &buttonInterrupt, FALLING); //set the interrupt on the switch to activate when going from HIGH to LOW

  if (esp_now_init() != ESP_OK) {
    Serial.println("Failed to initialize");
    return; // Initialize ESP-NOW and check for success
  }
  
  esp_now_register_send_cb(onDataSent); // Register the send callback function

  esp_now_peer_info_t peerInfo; // Data structure for handling peer information

  // Copy the receiver's MAC address to peer information
  memset(&peerInfo, 0, sizeof(peerInfo));
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);  

  peerInfo.channel = 0; // Set WiFi channel to 0 (default)
  peerInfo.encrypt = false; // Disable encryption

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;// Add peer and check for success
  }
  
  Serial.println("Successfully paired");
}

void loop() {
  if (buttonPressed) {
    Serial.println("What to send:");
    uint8_t buffer[16] = {0};
    
    // Wait until Serial input
    while(Serial.available() == 0){}

    // Place entered string into buffer array and determine length of entered string
    int inputLength = Serial.readBytesUntil('\r', buffer, 16) -1;

    // Clear remaining bytes in Serial Monitor
    while(Serial.available() > 0) {
      char t = Serial.read();
      t = t ;  // Avoid compiler warnings about how t is "unused"
    }

    uint8_t *data = &buffer[0]; //copy the address of the buffer array to put in the esp_now_send function
    
    buttonPressed = false;

    Serial.println("Sending now");
    esp_now_send(broadcastAddress, data, inputLength); // send the message to the given MAC address

    delay(1000);
  }
} 