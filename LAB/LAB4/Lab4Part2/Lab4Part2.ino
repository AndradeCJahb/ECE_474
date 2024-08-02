
setup()
// ====================> TODO:
//         1. Initialize pins, serial, LCD, etc
//         2. Create binary semaphore for synchronizing light level data.
//         3. Create Tasks
//          - Create the `Light Detector Task` and assign it to Core 0.
//          - Create `LCD Task` and assign it to Core 0.
//          - Create `Anomaly Alarm Task` and assign it to Core 1.
//          - Create `Prime Calculation Task` and assign it to Core 1.
loop()


Light Detector Task (Core 0)
// ====================> TODO:
//          1. Initialize Variables
//          2. Loop Continuously
//           - Read light level from the photoresistor.
//           - Take semaphore
//           - Calculate the simple moving average and update variables.
//           - Give semaphore to signal data is ready.


LCD Task (Core 0)
// ====================> TODO:
//          1. Initialize Variables
//           2. Loop Continuously
//            - Wait for semaphore.
//            - If data has changed, update the LCD with the new light level and SMA.
//            - Give back the semaphore.


Anomaly Alarm Task (Core 1)
// ====================> TODO:
//            1. Loop Continuously
//             - Wait for semaphore.
//             - Check if SMA indicates a light anomaly (outside thresholds).
//             - If anomaly detected, play sound on buzzer.
//             - Give back the semaphore.


Prime Calculation Task (Core 1)
// ====================> TODO:
//            1. Loop from 2 to 5000
//             - Check if the current number is prime.
//             - If prime, print the number to the serial monitor.
