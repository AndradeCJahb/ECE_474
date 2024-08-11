// Filename:    ICTE8.ino
// Author:      Christopher Andrade (2221525), Theo Favour (2169814)
// Date:        8/5/2024
// Description: This file takes input from the Serial monitor and sets the duty cycle of an offboard LED to the aforementioned input utilizing a queue.

#define BUZZER_PIN 3     

TaskHandle_t vATaskHandle;

struct AMessage {
char ucMessageID;
char ucData[ 20 ];
} xMessage;

uint32_t ulVar = 10UL;

void vATask( void *pvParameters ) {
    QueueHandle_t xQueue1, xQueue2;
    struct AMessage *pxMessage;

    xQueue1 = xQueueCreate( 10, sizeof( uint32_t ) );
    xQueue2 = xQueueCreate( 10, sizeof( struct AMessage * ) );

    if( xQueue1 != 0 ) {
        // Send an uint32_t.  Wait for 10 ticks for space to become
        // available if necessary.
        if( xQueueGenericSend( xQueue1, ( void * ) &ulVar, ( TickType_t ) 10, queueSEND_TO_BACK ) != pdPASS ) {
            // Failed to post the message, even after 10 ticks.
        }
    }

    if( xQueue2 != 0 ) {
        // Send a pointer to a struct AMessage object.  Don't block if the
        // queue is already full.
        pxMessage = & xMessage;
        xQueueGenericSend( xQueue2, ( void * ) &pxMessage, ( TickType_t ) 0, queueSEND_TO_BACK );
    }

}


void setup() {
    Serial.begin(115200);
    while(!Serial);
    
    xTaskCreate(vATask, "vATask" , 2024, NULL, 1, &vATaskHandle);
    xTaskCreate(ledTask, "ledTask", 2024, NULL, 1, &ledTaskHandle);
}