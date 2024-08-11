#include <esp_now.h>
#include <WiFi.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Callback function called when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Send Success" : "Send Fail");
}

// Task to handle ESP-NOW communication
void espNowTask(void *pvParameter) {
  // Set the Wi-Fi mode to Station (client mode)
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW protocol
  esp_now_init();

  // Register the callback function for data sent event
  esp_now_register_send_cb(OnDataSent);

  // Define the peer's MAC address (destination address)
  uint8_t broadcastAddress[] = {0x24, 0x6F, 0x28, 0x3A, 0xBE, 0xE4};

  // Initialize peer information structure
  esp_now_peer_info_t peerInfo;
  memset(&peerInfo, 0, sizeof(peerInfo));
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);

  // Add the peer to the ESP-NOW network
  esp_now_add_peer(&peerInfo);

  // Send a message to the peer
  esp_now_send(broadcastAddress, (uint8_t *)"Hello", sizeof("Hello"));

  // Task is complete, delete it
  vTaskDelete(NULL);
}

// Main application entry point
extern "C" void app_main() {
  // Initialize serial communication for debugging purposes
  Serial.begin(115200);

  // Create a FreeRTOS task to run the ESP-NOW operations
  xTaskCreate(&espNowTask, "espNowTask", 4096, NULL, 5, NULL);
}
