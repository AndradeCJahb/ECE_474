#include <SPI.h>
#include <LoRa.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Task to handle LoRa communication
void loRaTask(void *pvParameter) {
  // Initialize LoRa with the specified frequency
  LoRa.begin(915E6);

  // Loop to continuously send LoRa packets
  while (1) {
    // Begin a LoRa packet
    LoRa.beginPacket();

    // Write data to the LoRa packet
    LoRa.print("Hello World");

    // End and send the LoRa packet
    LoRa.endPacket();

    // Delay for 1 second before sending the next packet
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// Main application entry point
extern "C" void app_main() {
  // Initialize serial communication for debugging purposes
  Serial.begin(115200);

  // Create a FreeRTOS task to run the LoRa operations
  xTaskCreate(&loRaTask, "loRaTask", 4096, NULL, 5, NULL);
}
