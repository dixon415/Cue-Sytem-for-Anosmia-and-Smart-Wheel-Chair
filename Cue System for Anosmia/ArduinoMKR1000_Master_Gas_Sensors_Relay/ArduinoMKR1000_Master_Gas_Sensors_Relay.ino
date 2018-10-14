/* This is the code for the 'Cue System for Anosmia' part of my project deisgned for the Element14 Community's 'Design for a Cause' design challenge, 
 *  sponsored by Arduino. In particular, this is for the main smell sensing unit with Bluetooth Master Arduino MKR1000.
 * 
 * Design Challenge Page - https://www.element14.com/community/community/design-challenges/designforacause/blog
 * 
 * Blog Link - https://www.element14.com/community/community/design-challenges/designforacause/blog/2018/08/03/cue-system-for-anosmia-and-smart-wheelchair-8-gas-sensors
 * 
 * Author : Dixon Selvan
 * Date   : September 09, 2018
 * Project: Cue system for Anosmia and Smart WheelChair
 * Website: 
 * 
 * Hardware components Required (Extra Unit)
 * ----------------------------------------
 * 1. Arduino MKR1000 board
 * 2. Gas Sensors (MQ-2 & MQ-5) pair - x2
 * 3. Bluetooth module
 * 4. ProtoShield/ Breadboard and Few Jumper wires
 * 5. 12V Power Adapter
 * 6. DC Fan
 * 
 * Connections
 * -----------
 * Arduino MKR1000  |   Bluetooth Module
 * ---------------------------------------------
 *      5V          |         +5V
 *      Gnd         |         Gnd
 *      RX          |         RX     
 *      TX          |         TX
 * Arduino MKR1000  |   Gas Sensors (MQ-2 & MQ-5)
 * ---------------------------------------------
 *      5V          |         VCC
 *      Gnd         |         Gnd
 *      -           |         D0     
 *      A0/A4/A5/A6 |         A0
 * Arduino MKR1000  |   LED (Green/ Red)
 * ---------------------------------------------
 *      D1          |   Positive of Green LED      
 *      D2          |   Positive of Red LED
 *      Gnd         |   Negative of both the LEDs
 *
 * Connecting Arduino MKR1000 with thinger.io, getting started guide - http://docs.thinger.io/arduino/
 */

/*Disabling the secure TLS/SSL connection*/
#define _DISABLE_TLS_

#include <SPI.h>
#include <WiFi101.h>
#include <ThingerWifi101.h>
#include "arduino_secrets.h"

/*Please enter your sensitive data and credential in the Secret tab/arduino_secrets.h*/
char ssid[] = SECRET_SSID;  // Your network SSID (name)
char pass[] = SECRET_PASS;  // Your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;           // Your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
/*Find instructions to obtain the below value here - https://www.element14.com/community/community/design-challenges/designforacause/blog/2018/08/16/cue-system-for-anosmia-and-smart-wheelchair-5-fall-and-collision-detection#jive_content_id_Obtaining_the_Webhook_URL_to_trigger_an_event_from_Arduino_MKR1000  */
char server[] = "maker.ifttt.com"; 

/* Initialize the Ethernet client library with the IP address and port of the server that you want to connect to (port 80 is default for HTTP):*/
WiFiClient client;

/*Declaring Variables*/
int relayState = 0;
int collisionLED = 0;     //Red LED to Alert user
int statusLED = 1;        //Green LED for status

/*Create an account in thinger.io and replace username below with that username. 
Create a new device and replace deviceId, deviceCredential below with the one you had created.*/
ThingerWifi101 thing(SECRET_USERNAME, SECRET_DEVICEID, SECRET_DEVICECREDENTIAL);

void setup() {
  pinMode(collisionLED, OUTPUT);
  pinMode(statusLED, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(A6, INPUT);
    
  /*Serial initialisation*/
  Serial1.begin(38400);
  
  /*Replace the below accrodingly with your WiFi SSID and password*/
  thing.add_wifi(SECRET_SSID, SECRET_PASS);

  /*The relay state is obtained as an input resource (integer/ number) from thinger.io*/
  thing["Relay State"] << [](pson& in){
      relayState = in["state"];
      changeRelayState();
  };

  /*The gas sensor values are sent as an output resource to the thinger.io*/
  thing["MQ-5 (E)"] >> [](pson& out){
      out = analogRead(A0);
  };

  thing["MQ-2 (E)"] >> [](pson& out){
      out = analogRead(A6);
  };

  thing["MQ-5 (M)"] >> [](pson& out){
      out = analogRead(A4);
  };

  thing["MQ-2 (M)"] >> [](pson& out){
      out = analogRead(A5);
  };

  /*Check for the presence of the WiFi shield:*/
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    /*Don't continue, to inidcate code is stuck here, turn on both the LEDs*/
    digitalWrite(collisionLED, HIGH);
    digitalWrite(statusLED, HIGH);
    while (true);
  }

  /*Attempt to connect to WiFi network:*/
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    digitalWrite(collisionLED, HIGH);
    Serial.println(ssid);
    /*Connect to WPA/WPA2 network. Change this line if using open or WEP network:*/
    status = WiFi.begin(ssid, pass);
    /*wait 10 seconds for connection:*/
    delay(10000);
  }

  Serial.println("Connected to wifi");
  printWiFiStatus();
  delay(5);
}

void loop() {
  thing.handle();
  sendToGoogleSheetsExtraUnit();
  sendToGoogleSheetsMainUnit();
  delay(1000);
}

void changeRelayState(){
  if(relayState == 1)
  {
    digitalWrite(statusLED, HIGH);
    Serial1.print(1);
  }
  else if (relayState == 2){
    digitalWrite(statusLED, LOW);
    Serial1.print(2);
  }
  else if(relayState == 3)
  {
    digitalWrite(statusLED, HIGH);
    Serial1.print(3);
  }
  else if (relayState == 4){
    digitalWrite(statusLED, LOW);
    Serial1.print(4);
  }
  else  if(relayState == 5)
  {
    digitalWrite(statusLED, HIGH);
    Serial1.print(5);
  }
  else if (relayState == 6){
    digitalWrite(statusLED, LOW);
    Serial1.print(6);
  }
}

void printWiFiStatus() {
  /*Print the SSID of the network you're attached to:*/
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  /*pPrint your WiFi shield's IP address:*/
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  /*Print the received signal strength:*/
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void sendToGoogleSheetsExtraUnit(){
  int v1 = analogRead(A0);
  int v2 = analogRead(A6);
   
  String df1 = "{\"value1\":";
  String df2 = "\"value2\":";
  String df3 = "\"value3\":";
  String IFTTT_POST_DATA = df1 + "\"" + String(v1) + "\"" + "," + df2 + "\"" + String(v2) + "\"" + "}" ;
  String IFTTT_POST_DATA_SIZE = String(IFTTT_POST_DATA.length());
  
  Serial.println("\nStarting connection to server...");
  /*If you get a connection, report back via serial:*/
  if (client.connect(server, 80)) {
    
    
    Serial.println("connected to server");
    /*Make a HTTP request:
    Find instructions to obtain the below value here - https://www.element14.com/community/community/design-challenges/designforacause/blog/2018/08/16/cue-system-for-anosmia-and-smart-wheelchair-5-fall-and-collision-detection#jive_content_id_Obtaining_the_Webhook_URL_to_trigger_an_event_from_Arduino_MKR1000 */
    client.println("POST /trigger/{YOUR_IFTTT_EVENT_NAME}/with/key/{YOUR_IFTTT_KEY} HTTP/1.1");
    client.println("Host: maker.ifttt.com");
    client.println("Connection: close");
    client.println("Content-Type: application/json");
    client.println("Content-Length: "+IFTTT_POST_DATA_SIZE);
    client.println();
    client.println(IFTTT_POST_DATA);

    digitalWrite(statusLED, HIGH);
  }

  int reqReceived = 1;

  while(reqReceived){
  
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
      reqReceived = 0;
    }
  
//    /*If the server's disconnected, stop the client:*/
//    if (!client.connected()) {
//      Serial.println();
//      Serial.println("disconnecting from server.");
//      client.stop();
//      digitalWrite(statusLED, LOW);
//    }
  }
}

void sendToGoogleSheetsMainUnit(){
  int v1 = analogRead(A4);
  int v2 = analogRead(A5);
   
  String df1 = "{\"value1\":";
  String df2 = "\"value2\":";
  String df3 = "\"value3\":";
  String IFTTT_POST_DATA = df1 + "\"" + String(v1) + "\"" + "," + df2 + "\"" + String(v2) + "\"" + "}" ;
  String IFTTT_POST_DATA_SIZE = String(IFTTT_POST_DATA.length());
  
  Serial.println("\nStarting connection to server...");
  /*If you get a connection, report back via serial:*/
  if (client.connect(server, 80)) {
    
    
    Serial.println("connected to server");
    /*Make a HTTP request:
    Find instructions to obtain the below value here - https://www.element14.com/community/community/design-challenges/designforacause/blog/2018/08/16/cue-system-for-anosmia-and-smart-wheelchair-5-fall-and-collision-detection#jive_content_id_Obtaining_the_Webhook_URL_to_trigger_an_event_from_Arduino_MKR1000 */
    client.println("POST /trigger/{YOUR_IFTTT_EVENT_NAME}/with/key/{YOUR_IFTTT_KEY} HTTP/1.1");
    client.println("Host: maker.ifttt.com");
    client.println("Connection: close");
    client.println("Content-Type: application/json");
    client.println("Content-Length: "+IFTTT_POST_DATA_SIZE);
    client.println();
    client.println(IFTTT_POST_DATA);

    digitalWrite(statusLED, HIGH);
  }

  int reqReceived = 1;

  while(reqReceived){
  
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
      reqReceived = 0;
    }
  
//    /*If the server's disconnected, stop the client:*/
//    if (!client.connected()) {
//      Serial.println();
//      Serial.println("disconnecting from server.");
//      client.stop();
//      digitalWrite(statusLED, LOW);
//    }
  }
}
