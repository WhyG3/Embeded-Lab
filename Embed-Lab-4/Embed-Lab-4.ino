#include <WiFi.h>
#include "ThingSpeak.h" //for HTTP protoco communication between Kidbrightand ThingSpeak server
#include <PubSubClient.h> //for  MQTT protocol communication between Kidbright and ThingSpeak server
#include <Wire.h> 

#define LM73_ADDR 0x4D
const int analogInPin = 36; 


#define SEND_DELAY 5000
char ssid[] = "casa21";                    //Wifi's name
char pass[] = "";                         //Wifi's password

#define channelID 1691830  
const char * myWriteAPIKey = "AGN0KW528UQVHGJ7";    //HTTP's API KEY
//MQTT 
const char mqttUserName[] = "PDwpFy8FMhgRBDopPBwOJSI"; 
const char clientID[] = "PDwpFy8FMhgRBDopPBwOJSI";
const char mqttPass[] = "+AoVImv2eyQOYSB6bfOJrAnp";
#define mqttPort 1883

WiFiClient client;
PubSubClient mqttClient( client );

//sensor
int keyIndex = 0;
int sensorLDR_Value = 0;      
int LDR_Value = 0; 
float tempData;
float readTemperature();
float readLDR();
//MQTT protocol
const char* server = "mqtt3.thingspeak.com";
int status = WL_IDLE_STATUS;
long lastPublishMillis = 0;
int connectionDelay = 1;
int updateInterval = 15;


void setup() {
  Serial.begin(115200);  //Initialize serial
  // Delay to allow serial monitor to come up.
  delay(3000);
  Wire.begin(4, 5);
  analogReadResolution(10);
  // Connect to Wi-Fi network.
  connectWifi();
  // Configure the MQTT client
  mqttClient.setServer( server, mqttPort );
  // Set the buffer to handle the returned JSON. NOTE: A buffer overflow of the message buffer will result in your callback not being invoked.
  mqttClient.setBufferSize( 4069 );
  ThingSpeak.begin(client);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

}

void loop() {
  // Reconnect to WiFi if it gets disconnected.
  if (WiFi.status() != WL_CONNECTED) {
      connectWifi();
  }
  
  int ldr_value=readLDR();
  tempData = readTemperature();
  //Connect 
  ThingSpeak.setField(1, tempData);
  ThingSpeak.setField(2, LDR_Value);
  thingspeak_multi_write();

  // Connect if MQTT client is not connected and resubscribe to channel updates.
  if (!mqttClient.connected()) {
     mqttConnect(); 
  }
  
  // Call the loop to maintain connection to the server.
  mqttClient.loop(); 
  
  // Update ThingSpeak channel periodically. The update results in the message to the subscriber.
  if ( abs(millis() - lastPublishMillis) > updateInterval*1000) {
    mqttPublish( channelID, (String("field3=" + String(tempData) + "&field4=" + String(LDR_Value) )));
    lastPublishMillis = millis();
  }
  Serial.print("\t LDR sensor output = ");  Serial.println(LDR_Value); 
  Serial.print("\t Temperature sensor output = ");  Serial.println(tempData); 
  Serial.println("5 s");
  delay(SEND_DELAY); 
  
}

// Connect to WiFi.
void connectWifi()
{
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }
}
// HTTP function

void thingspeak_single_write(byte ch, int value) {
  int x = ThingSpeak.writeField(channelID, ch, value, myWriteAPIKey);
  if (x == 200) {
    Serial.println("Channe " + String(ch) + " update value " + String(value) + " successful.");
  }
  else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
}

void thingspeak_multi_write() {
  int x = ThingSpeak.writeFields(channelID, myWriteAPIKey);
  if (x == 200) {
    Serial.println("Multichannel update successful.");
  }
  else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
}

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
float readLDR(){
  sensorLDR_Value = analogRead(analogInPin); 
  LDR_Value = map(sensorLDR_Value, 0, 1023, 0, 99);
//  Serial.print("\t light output = ");  Serial.println(LDR_Value); 
  return LDR_Value;
}

//MQTT function 

// Connect to MQTT server.
void mqttConnect() {
  // Loop until connected.
  while ( !mqttClient.connected() )
  {
    // Connect to the MQTT broker.
    if ( mqttClient.connect( clientID, mqttUserName, mqttPass ) ) {
      Serial.print( "MQTT to " );
      Serial.print( server );
      Serial.print (" at port ");
      Serial.print( mqttPort );
      Serial.println( " successful." );
    } else {
      Serial.print( "MQTT connection failed, rc = " );
      // See https://pubsubclient.knolleary.net/api.html#state for the failure code explanation.
      Serial.print( mqttClient.state() );
      Serial.println( " Will try again in a few seconds" );
      delay( connectionDelay*1000 );
    }
  }
}

// Publish messages to a ThingSpeak channel.
void mqttPublish(long pubChannelID, String message) {
  String topicString ="channels/" + String( pubChannelID ) + "/publish";
  mqttClient.publish( topicString.c_str(), message.c_str() );
}
