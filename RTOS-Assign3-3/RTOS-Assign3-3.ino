
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

// You'll likely need this on vanilla FreeRTOS
//#include semphr.h

// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

// Pins (change this if your Arduino board does not have LED_BUILTIN defined)
static const int led_bluetooth= 17;
static const int led_wifi= 2;
static const int led_ntp= 15;
static const int led_iot= 12;

//*****************************************************************************
// Tasks

// Blink LED based on rate passed by parameter

//*****************************************************************************
// Main (runs as its own task with priority 1 on core 1)

void setup() {
  // Configure Serial
  Serial.begin(115200);
  pinMode(led_bluetooth, OUTPUT);
  pinMode(led_wifi, OUTPUT);
  pinMode(led_ntp, OUTPUT);
  pinMode(led_iot, OUTPUT);
  // Wait a moment to start (so we don't miss Serial output)
  xTaskCreatePinnedToCore(task0,"t0",1024,NULL,tskIDLE_PRIORITY+1,NULL,app_cpu);
  xTaskCreatePinnedToCore(task1,"t1",1024,NULL,tskIDLE_PRIORITY+1,NULL,app_cpu);
  xTaskCreatePinnedToCore(task2,"t1",1024,NULL,tskIDLE_PRIORITY+1,NULL,app_cpu);
  xTaskCreatePinnedToCore(task3,"t1",1024,NULL,tskIDLE_PRIORITY+1,NULL,app_cpu);

}

void loop() {
  // Do nothing but allow yielding to lower-priority tasks
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}
void task0( void *pvParameters ) {
   while(1) { 
      Serial.println(" bluetooth" );
      digitalWrite(led_bluetooth, HIGH);
      vTaskDelay( pdMS_TO_TICKS(300) );
      digitalWrite(led_bluetooth, LOW);
      vTaskDelay( pdMS_TO_TICKS(300) );
   }
}
void task1( void *pvParameters ) {
   while(1) { 
      Serial.println( "wifi" );
      digitalWrite(led_wifi, HIGH);
      vTaskDelay( pdMS_TO_TICKS(300) );
      digitalWrite(led_wifi, LOW);
      vTaskDelay( pdMS_TO_TICKS(300) );
   }
}
void task2( void *pvParameters ) {
   while(1) { 
      Serial.println( "iot" );
      digitalWrite(led_iot, HIGH);
      vTaskDelay( pdMS_TO_TICKS(300) );
      digitalWrite(led_iot, LOW);
      vTaskDelay( pdMS_TO_TICKS(300) );
   }
}
void task3( void *pvParameters ) {
   while(1) { 
      Serial.println( "ntp" );
      digitalWrite(led_ntp, HIGH);
      vTaskDelay( pdMS_TO_TICKS(300) );
      digitalWrite(led_ntp, LOW);
      vTaskDelay( pdMS_TO_TICKS(300) );
   }
}
