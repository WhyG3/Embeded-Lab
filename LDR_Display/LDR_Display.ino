#include <Wire.h> 
#include <Adafruit_GFX.h> 
#include "Adafruit_LEDBackpack.h" 
#define SEND_DELAY 200

const int analogInPin = 36; 
int sensorValue = 0;      
int outputValue = 0;  

Adafruit_8x16minimatrix matrix = Adafruit_8x16minimatrix(); 

void setup() { 
  Serial.begin(9600);
  analogReadResolution(10);

  matrix.begin(0x70);
  matrix.setRotation(1); 
  matrix.setTextSize(1); 
  matrix.setTextWrap(false); 
  matrix.setTextColor(LED_ON);
}

void loop() {
  // read the analog in value:
  sensorValue = analogRead(analogInPin); 
  outputValue = map(sensorValue, 0, 1023, 0, 99);  
 
  Serial.print("sensor = ");     Serial.print(sensorValue);  
  Serial.print("\t output = ");  Serial.println(outputValue); 

  //Display value on LED Matrix
  matrix.clear();
  matrix.setCursor(3,0);
  String response = String(outputValue);
  Serial.println(response);
  matrix.print(response); 
  matrix.writeDisplay();

  delay(SEND_DELAY); 
}
