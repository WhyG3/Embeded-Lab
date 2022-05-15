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

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println(LINE.getVersion());
  connectWifi();  // Connect to Wi-Fi network.
  // connect Line API service
  LINE.setToken(LINE_TOKEN);
  LINE.notify("Hellow");
  // sent number to line 
  int va = 25;
  float pi = 3.14159265359;
  float temp = 39.12345;
  LINE.notify(va);    // interger number
  LINE.notify(pi, 4); // 4 digit float number 

  // sent text to line
  LINE.notify("Temp = " + String(temp, 3) + " C");

  // sent sticket to line
  LINE.notifySticker("อุณหภูมิสูง", 4, 274); // sent Line Sticker by PackageID 1 , StickerID 2

  // sent image by url
  LINE.notifyPicture("https://cq.lnwfile.com/_/cq/_raw/0w/1o/gu.jpg");
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (WiFi.status() != WL_CONNECTED)
  {
    connectWifi();
  }
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
