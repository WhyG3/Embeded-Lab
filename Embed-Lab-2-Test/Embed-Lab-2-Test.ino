#include<Wire.h>
#define I2C_SDA 4
#define I2C_SCL 5
#define I2C_ADDR 0x4D
float Temp_value;
void setup() {
  // put your setup code here, to run once:
  Wire.begin(I2C_SDA, I2C_SCL);
  Serial.begin(115200);
}
void loop() {
  // put your main code here, to run repeatedly:
  byte Temp_data[2];
  int temp1 = 0;
  float temp0 = 0.0;
  // I2C start communicate with Temp sensor
  Wire.beginTransmission(I2C_ADDR);
  Wire.write(0);
  Wire.endTransmission();
  // get input from Temp sensor
  Wire.requestFrom(0x4D,2);
  Temp_data[1] = Wire.read();
  Temp_data[0] = Wire.read();
  temp1 = rawDataToCelsius1(Temp_data[1]);
  temp0 = rawDataToCelsius0(Temp_data[0]);
  Serial.print("Temp:");
  Serial.print(temp1+temp0);
  Serial.println(" °C");
  delay(2000);
}
float rawDataToCelsius1(byte number){ // Convert first 8 bits Raw data first  from LM73 to Celsius
  float temp1 = 0.000;
  uint8_t rawData = 0;
  int z = 0;
 for (int b = 7; b >= 0; b--) 
 {
   rawData = bitRead(number, b);   // should look at Datasheet of LM73
   if (rawData == 1){
     z = pow(2,b+1);
    temp1 += z; 
   }
 }
  return temp1;
}

float rawDataToCelsius0(byte number){ // Convert secound 8 bit Raw data  from LM73 to Celsius 
  float temp0 = 0.000;
  uint8_t rawData = 0;
  float z = 0;
  int minus = 7;
 for (int b = 7; b >=0 ; b--)
 {
   rawData = bitRead(number, b); // should look at Datasheet of LM73
   if (rawData == 1){
     z = pow(2,minus-b);
     z = 1/z;
    temp0 += z; 
   }
 }
  return temp0;
}
