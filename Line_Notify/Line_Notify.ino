/*
send temperature value to line notify
*/
// setup
#include <WiFi.h>
#include <Wire.h>

// setup line API
#include <TridentTD_LineNotify.h>
#define LINE_TOKEN "Ther4dP40dpBluO4LLATCrAwEM21Vh1k1nZw29qH0su"

// setup WIfi
char ssid[] = "casa21"; // Wifi's name
char pass[] = "";       // Wifi's password
// set up global variable
#define LM73_ADDR 0x4D // Kidbright's temerature sensor address
#define DELAY 10000     // 10 s
float data;
float readTemperature();

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire1.begin(4, 5);
  Serial.println(LINE.getVersion());
  connectWifi();  // Connect to Wi-Fi network.
  // connect Line API service
  LINE.setToken(LINE_TOKEN);
  LINE.notify("Hello");
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (WiFi.status() != WL_CONNECTED)
  {
    connectWifi();
  }
  
  data = readTemperature();
  Serial.println(data);
  LINE.notify("Temperature "+String(data)+" C");
  delay(DELAY);
  
}

// Function connect Wifi
void connectWifi()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to SSID");
    Serial.print(ssid);
    while(WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(DELAY);
    }
    Serial.printf("\nWiFi connected\nIP : ");
    Serial.println(WiFi.localIP());
  }
}
// read temp sensor function
float readTemperature() {
  Wire1.beginTransmission(LM73_ADDR);
  Wire1.write(0x00);
  Wire1.endTransmission();
 
  uint8_t count = Wire1.requestFrom(LM73_ADDR, 2);
  float temp = 0.0;
  if (count == 2) {
    byte buff[2];
    buff[0] = Wire1.read();
    buff[1] = Wire1.read();
    temp += (int)(buff[0]<<1);
    if (buff[1]&0b10000000) temp += 1.0;
    if (buff[1]&0b01000000) temp += 0.5;
    if (buff[1]&0b00100000) temp += 0.25;
    if (buff[0]&0b10000000) temp *= -1.0;
  }
  return temp;
} 
