#include <Arduino_FreeRTOS.h>
// see: https://github.com/feilipu/Arduino_FreeRTOS_Library
#define LED_PIN  13 // PORTB5 pin
void task( void *pvParameters );
void setup() {
  xTaskCreate(
    task /* task function */,
    "BlinkTask" /* task name */, 
    128 /* stack size */,
    NULL  /* no parameters */, 
    2 /* priority level */, 
    NULL /* no task handle */ );
   // Note the FreeRTOS task scheduler is started automatically.  
}
void loop() {}
void task( void *pvParameters ) {
   boolean state = false;
   pinMode( LED_PIN, OUTPUT );
   while(1) { // toggle and update LED output
      digitalWrite( LED_PIN, state = !state );
      vTaskDelay( 1 );
   }
}
//////////////////////////////////////////////////////////////////
