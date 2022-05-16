 
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <TridentTD_LineNotify.h>




#define BUZZER_PIN 13
#define WIFI_LED 2
#define IOT_LED 12
#define USB_IO 25
#define BT_LED 23

#define LM73_ADDR 0x4D


#define SSID        "Y.5G"   // ชื่อ Wifi
#define PASSWORD    "y5g_stark"   // รหัส Wifi
#define LINE_TOKEN  "WoU89RimCydkbdRdRQingDYEHIKrJ6RcYvdbwbA0HJb"   // TOKEN

int HOUR = 0;
int MIN  = 0 ;
int Select = -1;
int Select_HOUR_1; //Select 1
int Select_minute_1;
int Select_HOUR_2; //Select 2
int Select_minute_2;
int Select_HOUR_3; //Select 3
int Select_minute_3;
TickType_t DELAY(unsigned long ms) /* เรียกการคำนวณ Delay RTOS */
{
    return ms / portTICK_PERIOD_MS;
}

int analog_value = 0;
double temp=0;
//byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;




Adafruit_8x16minimatrix matrix = Adafruit_8x16minimatrix();
//Clock config//
#define MCP7941x_RTC_I2C_ADDR 0x6F
#define RTC_LOCATION 0x00
#define WireSend(x) Wire1.write(x)
#define WireReceive() Wire1.read()

/////////////////


byte decToBcd(byte val)
{

 return ( (val/10*16) + (val%10) );

}


byte bcdToDec(byte val)
{

 return ( (val/16*10) + (val%16) );

}



void setDateTime(
  byte second,        // 0-59
  byte minute,        // 0-59
  byte hour,          // 1-23
  byte dayOfWeek,     // 1-7
  byte dayOfMonth,    // 1-28/29/30/31
  byte month,         // 1-12
  byte year)          // 0-99
{
  Wire1.beginTransmission(MCP7941x_RTC_I2C_ADDR);
  WireSend(RTC_LOCATION);

  WireSend(decToBcd(second) & 0x7f);              // set seconds and disable clock (01111111)
  WireSend(decToBcd(minute) & 0x7f);              // set minutes (01111111)
  WireSend(decToBcd(hour) & 0x3f);                // set hours and to 24hr clock (00111111)
  WireSend(0x08 | (decToBcd(dayOfWeek) & 0x07));  // set the day and enable battery backup (00000111)|(00001000)
  WireSend(decToBcd(dayOfMonth) & 0x3f);          // set the date in month (00111111)
  WireSend(decToBcd(month) & 0x1f);               // set the month (00011111)
  WireSend(decToBcd(year));                       // set the year (11111111)

  Wire1.endTransmission();

  // Start Clock:
  Wire1.beginTransmission(MCP7941x_RTC_I2C_ADDR);
  WireSend(RTC_LOCATION);
  WireSend(decToBcd(second) | 0x80);     // set seconds and enable clock (10000000)
  Wire1.endTransmission();
}


void enableClock()
{
  // Get the current seconds value as the enable/disable bit is in the same
  // byte of memory as the seconds value:
  Wire1.beginTransmission(MCP7941x_RTC_I2C_ADDR);
  WireSend(RTC_LOCATION);
  Wire1.endTransmission();

  Wire1.requestFrom(MCP7941x_RTC_I2C_ADDR, 1);

  int second = bcdToDec(WireReceive() & 0x7f);  // 01111111

  // Start Clock:
  Wire1.beginTransmission(MCP7941x_RTC_I2C_ADDR);
  WireSend(RTC_LOCATION);
  WireSend(decToBcd(second) | 0x80);     // set seconds and enable clock (10000000)
  Wire1.endTransmission();
}



void getDateTime(
  byte *second,
  byte *minute,
  byte *hour,
  byte *dayOfWeek,
  byte *dayOfMonth,
  byte *month,
  byte *year)
{
  Wire1.beginTransmission(MCP7941x_RTC_I2C_ADDR);
  WireSend(RTC_LOCATION);
  Wire1.endTransmission();

  Wire1.requestFrom(MCP7941x_RTC_I2C_ADDR, 7);

  // A few of these need masks because certain bits are control bits
  *second     = bcdToDec(WireReceive() & 0x7f);  // 01111111
  *minute     = bcdToDec(WireReceive() & 0x7f);  // 01111111
  *hour       = bcdToDec(WireReceive() & 0x3f);  // 00111111
  *dayOfWeek  = bcdToDec(WireReceive() & 0x07);  // 01111111
  *dayOfMonth = bcdToDec(WireReceive() & 0x3f);  // 00111111
  *month      = bcdToDec(WireReceive() & 0x1f);  // 00011111
  *year       = bcdToDec(WireReceive());         // 11111111
  
}

void displayDateTime(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year) {
  
  if (hour < 10) {
    Serial.print("0");
  }

  Serial.print(hour, DEC);
  Serial.print(":");

  if (minute < 10) {
    Serial.print("0");
  }

  Serial.print(minute, DEC);
  Serial.print(":");

  if (second < 10) {
    Serial.print("0");
  }

  Serial.print(second, DEC);
  Serial.print("  ");

  if (dayOfMonth < 10) {
    Serial.print("0");
  }

  Serial.print(dayOfMonth, DEC);
  Serial.print("/");

  if (month < 10) {
    Serial.print("0");
  }

  Serial.print(month, DEC);
  Serial.print("/");

  Serial.print(year, DEC);

  Serial.print(" (");

  switch (dayOfWeek) {
    case 1:
      Serial.print("Sunday");
      break;

    case 2:
      Serial.print("Monday");
      break;

    case 3:
      Serial.print("Tuesday");
      break;

    case 4:
      Serial.print("Wednesday");
      break;

    case 5:
      Serial.print("Thursday");
      break;

    case 6:
      Serial.print("Friday");
      break;

    case 7:
      Serial.print("Saturday");
      break;
  }

  Serial.println(")");

     matrix.setTextSize(1);
     matrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
     matrix.setTextColor(LED_ON);
     matrix.setRotation(1);
  for (int8_t x=7; x>=-46; x--) {
    matrix.clear();
    matrix.setCursor(x,0);
    //Serial.print(hour);
    //Serial.print(minute);
    matrix.print(String(hour)+":"+String(minute));
    matrix.writeDisplay();
    delay(50);
  }   

}





///////////read temperature/////////////////

float readTemperature() {
  Wire1.beginTransmission(LM73_ADDR);
  Wire1.write(0x00); // Temperature Data Register
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




///show Temp and Light  ///////////////

void measure(){


      matrix.setTextSize(1);
      matrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
      matrix.setTextColor(LED_ON);
      matrix.setRotation(1);
      temp = readTemperature();
      Serial.print("Temp:");Serial.println(temp);
      analog_value = analogRead(36);
      if(analog_value > 1000){
        analog_value = 1000;
      }
      




      int ll = (1000 - analog_value)*100/1000;
      String Kidrightsensordata = "T: " + String(temp) + " C " + "L: " + String(ll) + " %";
      for (int8_t x=7; x>=-126; x--) {
        matrix.clear();
        matrix.setCursor(x,0);
        //matrix.print(Kidrightsensordata);
        matrix.writeDisplay();
        delay(50);
      }



}





void setup() {
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  //while (!Serial);
  Serial.begin(9600);
  pinMode(16,INPUT_PULLUP);
  pinMode(14,INPUT_PULLUP);
  pinMode(13, OUTPUT);
  Wire1.begin(4, 5);

  pinMode(WIFI_LED, OUTPUT); 
  pinMode(IOT_LED, OUTPUT); 
  pinMode(USB_IO, OUTPUT); 
  pinMode(BT_LED, OUTPUT); 
  digitalWrite(WIFI_LED, HIGH);
  digitalWrite(IOT_LED, HIGH);
  digitalWrite(USB_IO, HIGH);
  digitalWrite(BT_LED, HIGH);

  Serial.println(LINE.getVersion());
  WiFi.begin(SSID, PASSWORD);
  Serial.printf("WiFi connecting ",  SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.printf("\nWiFi connected\nIP : ");
  Serial.println(WiFi.localIP());
  LINE.setToken(LINE_TOKEN);

  matrix.begin(0x70);  // pass in the address
  minute = MIN;
  hour = HOUR;
  dayOfWeek = 3;   // 1 = Sunday, 7 = Saturday
  dayOfMonth = 8;
  month = 11;
  year = 11;
  setDateTime(second, minute, hour, dayOfWeek, dayOfMonth, month, year);
 // enableClock();
  getDateTime(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  displayDateTime(second, minute, hour, dayOfWeek, dayOfMonth, month, year);
}


void loop() {
//Setting time
 byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
        if (digitalRead(16)==LOW and digitalRead(14) ==LOW){
     //Select
             Select = 0;
             for (int8_t x=7; x>=-36; x--) {
                matrix.clear();
                matrix.setCursor(x,0);
                matrix.print("Select");
                matrix.writeDisplay();
                delay(75);
    }
                matrix.setCursor(11,0);
                matrix.print(String(Select));
                matrix.writeDisplay();
                         
            for (;;) {
    
            if (digitalRead(16)==LOW ){
          
              if (Select >= 3) {
                Select =-1;
                }
              Select += 1;
              matrix.setTextSize(1);
              matrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
              matrix.setTextColor(LED_ON);
              matrix.setRotation(1);
              matrix.clear();
              matrix.setCursor(11,0);
              matrix.print(Select);
              matrix.writeDisplay();
            delay(300);
          }

          //SET Hours
          if(digitalRead(14)==LOW){
             for (int8_t x=7; x>=-36; x--) {
                matrix.clear();
                matrix.setCursor(x,0);
                matrix.print("Hours");
                matrix.writeDisplay();
                delay(75);
    }
          
                matrix.clear();
                if (HOUR < 10){
                  matrix.setCursor(5,0);
                  matrix.print("0"+String(HOUR));
                  matrix.writeDisplay();
                  
                }
                if (HOUR >= 10){
                  matrix.setCursor(5,0);
                  matrix.print(String(HOUR));
                  matrix.writeDisplay();
                }

            

            for (;;) {
    //SET min
            if (digitalRead(16)==LOW ){
          
              if (HOUR >= 23) {
                HOUR =-1;
                }
              HOUR += 1;
              Serial.print(HOUR);
              matrix.setTextSize(1);
              matrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
              matrix.setTextColor(LED_ON);
              matrix.setRotation(1);
              matrix.clear();
            if (HOUR < 10){
              matrix.setCursor(5,0);
              matrix.print("0"+String(HOUR));
              matrix.writeDisplay();
              }
            if (HOUR >= 10){
              matrix.setCursor(5,0);
              matrix.print(HOUR);
              matrix.writeDisplay();
              }
            delay(300);
          }

          
          if(digitalRead(14)==LOW){
            for (int8_t x=7; x>=-36; x--) {
                matrix.clear();
                matrix.setCursor(x,0);
                matrix.print("Minute");
                matrix.writeDisplay();
                delay(75);
            }
            matrix.clear();
            if (MIN < 10){
              matrix.setCursor(5,0);
              matrix.print("0"+String(MIN));
              matrix.writeDisplay();
            }
            if (MIN >= 10){
              matrix.setCursor(5,0);
              matrix.print(MIN);
              matrix.writeDisplay();
            }

            for (;;) {    
            if (digitalRead(16)==LOW){
              if (MIN >= 59) {
                MIN =-1;
                }
              MIN += 1;
              Serial.print(MIN);
              matrix.setTextSize(1);
              matrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
              matrix.setTextColor(LED_ON);
              matrix.setRotation(1);
              matrix.clear();
              if (MIN < 10){
                matrix.setCursor(5,0);
                matrix.print("0"+String(MIN));
                matrix.writeDisplay();
              }
              if (MIN >= 10){
                matrix.setCursor(5,0);
                matrix.print(MIN);
                matrix.writeDisplay();
              }
              delay(300);
          }
           if(digitalRead(14)==LOW){
              goto theEnd;
            }
        }
      }
    }
  }
        }
        }
  //Action Select
   theEnd:
   Serial.println(Select);
   if (Select == 0) {
      matrix.setTextSize(1);
      matrix.setTextWrap(false);  // we dont want text to wrap so it scrolls nicely
      matrix.setTextColor(LED_ON);
      matrix.setRotation(1);
      for (int8_t x=7; x>=-86; x--) {
        matrix.clear();
        matrix.setCursor(x,0);
        matrix.print("Setting time to");
        matrix.writeDisplay();
        delay(50);
    }
    minute = MIN;
    hour = HOUR;
    setDateTime(second, minute, hour, dayOfWeek, dayOfMonth, month, year);
    getDateTime(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
    displayDateTime(second, minute, hour, dayOfWeek, dayOfMonth, month, year);
    LINE.notify("Setting time to "+String(hour)+":"+String(minute));
    Select = -1;
    }
   if (Select == 1) {
    Select_HOUR_1 = HOUR;
    Select_minute_1= MIN;
    LINE.notify("Alarm 1 Set time "+String(Select_HOUR_1)+":"+String(Select_minute_1));
    for (int8_t x=7; x>=-86; x--) {
    matrix.clear();
    matrix.setCursor(x,0);
    matrix.print("Alarm 1 SET");
    matrix.writeDisplay();
    delay(50);
    }
    Select = -1;
    }
   if (Select == 2) {
    Select_HOUR_2 = HOUR;
    Select_minute_2= MIN;
    for (int8_t x=7; x>=-86; x--) {
      matrix.clear();
      matrix.setCursor(x,0);
      matrix.print("Alarm 2 SET");
      matrix.writeDisplay();
      delay(50);
    }
    LINE.notify("Alarm 2 Set time "+String(Select_HOUR_2)+":"+String(Select_minute_2));
    Select = -1;
    }
   if (Select == 3) {
    Select_HOUR_3 = HOUR;
    Select_minute_3= MIN;
    LINE.notify("Alarm 3 Set time "+String(Select_HOUR_3)+":"+String(Select_minute_3));
    for (int8_t x=7; x>=-86; x--) {
      matrix.clear();
      matrix.setCursor(x,0);
      matrix.print("Alarm 3 SET");
      matrix.writeDisplay();
      delay(50);
    }
    Select = -1;
    }

    Serial.println(String(Select_HOUR_1)+":"+String(Select_minute_1)+":"+"Alarm 1");
    Serial.println(String(Select_HOUR_2)+":"+String(Select_minute_2)+":"+"Alarm 2");
    Serial.println(String(Select_HOUR_3)+":"+String(Select_minute_3)+":"+"Alarm 3");
    Serial.println(String(hour)+":"+String(minute)+":"+String(second));
   if (String(Select_HOUR_1)+":"+String(Select_minute_1) == String(hour)+":"+String(minute)) {
    if (int(second) < 10) {
      temp = readTemperature();
      analog_value = analogRead(36);
      if(analog_value > 1000){
        analog_value = 1000;
      }
      
      int ll = (1000 - analog_value)*100/1000;
      String Kidrightsensordata = "T: " + String(temp) + " C " + "L: " + String(ll) + " %";
      LINE.notify("Alarm 1 time : "+String(Select_HOUR_1)+":"+String(Select_minute_1));
      LINE.notify(String(Kidrightsensordata));
      }
      }



       if (String(Select_HOUR_2)+":"+String(Select_minute_2) == String(hour)+":"+String(minute)) {
        if (int(second) < 10) {
          temp = readTemperature();
          analog_value = analogRead(36);
          if(analog_value > 1000){
            analog_value = 1000;
          }
          
          int ll = (1000 - analog_value)*100/1000;
          String Kidrightsensordata = "T: " + String(temp) + " C " + "L: " + String(ll) + " %";
          LINE.notify("Alarm 2 time : "+String(Select_HOUR_2)+":"+String(Select_minute_2));
          LINE.notify(String(Kidrightsensordata));
          }
      }


       if (String(Select_HOUR_3)+":"+String(Select_minute_3) == String(hour)+":"+String(minute)) {
        if (int(second) < 10) {
          temp = readTemperature();
          analog_value = analogRead(36);
          if(analog_value > 1000){
            analog_value = 1000;
          }
          
          int ll = (1000 - analog_value)*100/1000;
          String Kidrightsensordata = "T: " + String(temp) + " C " + "L: " + String(ll) + " %";
          LINE.notify("Alarm 3 time : "+String(Select_HOUR_3)+":"+String(Select_minute_3));
          LINE.notify(String(Kidrightsensordata));
          }
      }
  measure();
  delay(100);
   getDateTime(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
 // delay(500);
  // Display the Date/Time on the serial line:
  displayDateTime(second, minute, hour, dayOfWeek, dayOfMonth, month, year);
//delay(500);




}
