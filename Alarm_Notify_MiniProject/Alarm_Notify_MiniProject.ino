/*
send temperature value to line notify
*/
// setup
#include <WiFi.h>
#include <Wire.h>
#define LM73_ADDR 0x4D // Kidbright's temerature sensor address
#define DELAY 5000     // 5 s
// setup line API
#include <TridentTD_LineNotify.h>
#define LINE_TOKEN "WoU89RimCydkbdRdRQingDYEHIKrJ6RcYvdbwbA0HJb"
// setup WIfi
char ssid[] = "casa21"; // Wifi's name
char pass[] = "";       // Wifi's password

// setup variable
float tempData; // data from temp sensor 


// -----------------------------------------------------  function -------------------------------------
float readTemperature();


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
// ------- Function Read Temperature from sensor -------
float readTemperature() {
  Wire.beginTransmission(LM73_ADDR);
  Wire.write(0x00);
  Wire.endTransmission();
 
  uint8_t count = Wire.requestFrom(LM73_ADDR, 2);
  float temp = 0.0;
  if (count == 2) {
    byte buff[2];
    buff[0] = Wire.read();
    buff[1] = Wire.read();
    temp += (int)(buff[0]<<1);
    if (buff[1]&0b10000000) temp += 1.0;
    if (buff[1]&0b01000000) temp += 0.5;
    if (buff[1]&0b00100000) temp += 0.25;
    if (buff[0]&0b10000000) temp *= -1.0;
  }
  return temp;
}

// ----------------------------------------------------- setup ----------------------------------
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin(4, 5);
  Serial.println(LINE.getVersion());
  connectWifi();  // Connect to Wi-Fi network.
  // connect Line API service
  LINE.setToken(LINE_TOKEN);
  LINE.notify("Hello");
  tempData=readTemperature();
  LINE.notify("Temp = " + String(tempData) + " C");

}

void loop()
{
  // put your main code here, to run repeatedly:
  if (WiFi.status() != WL_CONNECTED)
  {
    connectWifi();
  }
  tempData=readTemperature();
  // sent text to line
  delay(DELAY);
}

  
