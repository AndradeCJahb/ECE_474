#include <WiFi.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Wi-Fi network credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// Task to handle Wi-Fi connection
void wifiTask(void *pvParameter) {
  // Start the Wi-Fi connection using the provided credentials
  WiFi.begin(ssid, password);

  // Loop until the ESP32 is connected to the Wi-Fi network
  while (WiFi.status() != WL_CONNECTED) {
    // Delay for 1 second (1000 milliseconds) to avoid spamming the console
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    Serial.println("Connecting to Wi-Fi...");
  }

  // Print a message once connected to the Wi-Fi network
  Serial.println("Connected to Wi-Fi.");

  // Task is complete, delete it
  vTaskDelete(NULL);
}

// Main application entry point
extern "C" void app_main() {
  // Initialize serial communication for debugging purposes
  Serial.begin(115200);

  // Create a FreeRTOS task to run the Wi-Fi connection operations
  xTaskCreate(&wifiTask, "wifiTask", 4096, NULL, 5, NULL);
}
