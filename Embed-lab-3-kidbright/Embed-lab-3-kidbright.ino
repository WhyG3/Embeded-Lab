
#include <Wire.h>

#define MCP7941x_RTC_I2C_ADDR 0x6F
#define RTC_LOCATION 0x00
#define WireSend(x) Wire1.write(x)
#define WireReceive() Wire1.read()

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

void setTime(){
  String fromSerial;
  byte second;        // 0-59
  byte minute;        // 0-59
  byte hour;          // 1-23
  byte dayOfWeek;     // 1-7
  byte dayOfMonth;    // 1-28/29/30/31
  byte month;        // 1-12
  byte year;          // 0-99
   if (Serial.available()) {
      fromSerial = Serial.readStringUntil('\n');
      if (fromSerial == "set time") {
         Serial.println("Begining set time");
         Serial.println("Enter secound");
         delay(3000);
         while (Serial.available() <= 0);
         second = Serial.parseInt();
         Serial.print("secound : ");
         Serial.println(second);
         Serial.println("Enter minute");
         delay(3000);
         while (Serial.available() <= 0);
         minute = Serial.parseInt();
         Serial.print("minute : ");
         Serial.println(minute);
         Serial.println("Enter hour");
         delay(3000);
         while (Serial.available() <= 0);
         hour = Serial.parseInt();
         Serial.print("hour : ");
         Serial.println(hour);
         Serial.println("Enter day Of Week");
         delay(3000);
         while (Serial.available() <= 0);
         dayOfWeek = Serial.parseInt();
         Serial.print("day Of Week : ");
         Serial.println(dayOfWeek);
         Serial.println("Enter day Of Month");
         delay(3000);
         while (Serial.available() <= 0);
         dayOfMonth = Serial.parseInt();
         Serial.print("day Of Month : ");
         Serial.println(dayOfMonth);
         Serial.println("Enter Month");
         delay(3000);
         while (Serial.available() <= 0);
         month = Serial.parseInt();
         Serial.print("month : ");
         Serial.println(month);
         Serial.println("Enter year");
         delay(3000);
         while (Serial.available() <= 0);
         year = Serial.parseInt();
         Serial.print("year : ");
         Serial.println(year);
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
   }
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
}


void setup() {
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  //while (!Serial);
  Serial.begin(9600);

  Wire1.begin(4, 5);

  second = 0;
  minute = 25;
  hour = 18;
  dayOfWeek = 4;   // 1 = Sunday, 7 = Saturday
  dayOfMonth = 3;
  month = 9;
  year = 22;
  setDateTime(second, minute, hour, dayOfWeek, dayOfMonth, month, year);
  getDateTime(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  displayDateTime(second, minute, hour, dayOfWeek, dayOfMonth, month, year);
}

void loop() {
 
 byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  setTime();
  getDateTime(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  delay(500);
  // Display the Date/Time on the serial line:
  displayDateTime(second, minute, hour, dayOfWeek, dayOfMonth, month, year);
  delay(500);
}
