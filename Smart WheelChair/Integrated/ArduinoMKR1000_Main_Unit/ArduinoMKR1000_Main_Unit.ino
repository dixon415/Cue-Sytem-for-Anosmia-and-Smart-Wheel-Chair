/* This is the integrated code for the 'IoT Temperature measurement and monitoring', 'Fall and Collision Detection' and 'Home Appliance Control' sections of the 
 * Smart WheelChair deisgned for the Element14 Community's 'Design for a Cause' design challenge, sponsored by Arduino.
 * 
 * Design Challenge Page -
 * https://www.element14.com/community/community/design-challenges/designforacause/blog
 * 
 * This code contains extracts from the WiFi101 WebClient example code, 'created 13 July 2010 by dlf (Metodo2 srl) modified 31 May 2012 by Tom Igoe'
 * 
 * Author : Dixon Selvan
 * Date   : Sep 06, 2018
 * Project: Cue system for Anosmia and Smart WheelChair
 * Website: 
 * 
 * Hardware components Required (Main Unit)
 * ----------------------------------------
 * 1. Arduino MKR1000 board
 * 2. TMP36 temperature sensor
 * 3. HC-SR04 Ultrasonic Sensors - x2
 * 4. Accelerometer ADXL335Z Evaluation Board (any ADXL XXX should work)
 * 5. LEDs (Green/ Red) - x2
 * 6. Bluetooth module
 * 7. ProtoShield/ Breadboard and Few Jumper wires
 * 
 * Connections
 * -----------
 * Arduino MKR1000  |   TMP36 Temperature Sensor
 * ---------------------------------------------
 *      3.3V        |         +Vs
 *      Gnd         |         Gnd
 *      A1          |         Vout
 * Arduino MKR1000  |   Ultrasonic Sensor
 * ---------------------------------------------
 *      5V          |         Vcc
 *      Gnd         |         Gnd
 *      D2/3        |         Echo
 *      D5/4        |         Trig
 *      
 * Arduino MKR1000  |   ADXL335Z Evaluation Board
 * ---------------------------------------------
 *      VCC(3.3V)   |         VSS      
 *      Gnd         |         Gnd
 *      A2          |         Y
 *      A3          |         X
 *      
 * Arduino MKR1000  |   LED (Green/ Red)
 * ---------------------------------------------
 *      D0          |   Positive of Green LED      
 *      D1          |   Positive of Red LED
 *      Gnd         |   Negative of both the LEDs    
 * 
 * Blog Links 
 * ----------
 * 1. IoT Temperature Monitoring - https://www.element14.com/community/community/design-challenges/designforacause/blog/2018/07/28/cue-system-for-anosmia-and-smart-wheelchair-4-iot-temperature-monitoring-diy
 * 2. Fall and Collision Detection -  https://www.element14.com/community/community/design-challenges/designforacause/blog/2018/08/16/cue-system-for-anosmia-and-smart-wheelchair-5-fall-and-collision-detection
 * 3. Home Automation Control - https://www.element14.com/community/community/design-challenges/designforacause/blog/2018/09/05/cue-system-for-anosmia-and-smart-wheelchair-8b-home-appliance-control
 * 
 * Connecting Arduino MKR1000 with thinger.io, getting started guide - http://docs.thinger.io/arduino/
 * Create a new applet using IFTTT - https://ifttt.com/create
 * 
 */

#define _DISABLE_TLS_

/*Adding Required Libraries*/
#include <SPI.h>
#include <WiFi101.h>
#include <ThingerWifi101.h>
#include "arduino_secrets.h"

/*Please enter your sensitive data and credential in the Secret tab/arduino_secrets.h*/
char ssid[] = SECRET_SSID;  /* Your network SSID (name)*/
char pass[] = SECRET_PASS;  /* Your network password (use for WPA, or use as key for WEP)*/
int keyIndex = 0;           /* Your network key Index number (needed only for WEP)*/

/*GPIO pin assignments*/
int tempSensor = A1;
int XAxis = A3, YAxis = A2;
int statusLED = 0;    /*Green LED for status*/
int collisionLED = 1; /*Red LED to Alert user in case of collision*/
int trigPinF = 5, trigPinR = 4;
int echoPinF = 2, echoPinR = 3;

/*Variable declartaion and initialisation*/
int XValue = 0, YValue = 0;
int XFlag = 0, YFlag = 0;
int fallCount = 0;
int distance;
int measDistanceF;
int measDistanceR;
float sensorVoltage = 0.0;
float tempinC = 0.0;
float tempinF = 0.0;
unsigned long previousMillis = 0;
unsigned long previousMillis1 = 0;
long tempFeedDelay = 5000;
long collisionDelay = 100;
long duration;
int relayState = 0;

int status = WL_IDLE_STATUS;
/*Find instructions to obtain the below value here - https://www.element14.com/community/community/design-challenges/designforacause/blog/2018/08/16/cue-system-for-anosmia-and-smart-wheelchair-5-fall-and-collision-detection#jive_content_id_Obtaining_the_Webhook_URL_to_trigger_an_event_from_Arduino_MKR1000  */
char server[] = "maker.ifttt.com"; 
/* Initialize the Ethernet client library with the IP address and port of the server that you want to connect to (port 80 is default for HTTP):*/
WiFiClient client;

/*ThingerWifi101 thing("username", "deviceId", "deviceCredential");*/
ThingerWifi101 thing(SECRET_USERNAME, "ArduinoMKR1000", SECRET_DEVICECREDENTIAL);

/*************************************Setup*************************************/
void setup() {
  /*GPIO Desclartion*/
  pinMode(tempSensor, INPUT);
  pinMode(trigPinF, OUTPUT);
  pinMode(echoPinF, OUTPUT);
  pinMode(trigPinR, OUTPUT);
  pinMode(echoPinR, OUTPUT);
  pinMode(statusLED, OUTPUT);
  pinMode(collisionLED, OUTPUT);
  pinMode(XAxis, INPUT);
  pinMode(YAxis, INPUT);

  /*Serial Initialisation*/
  Serial.begin(38400);
  Serial1.begin(38400);

  /*Connecting to Wi-Fi - Thinger.io*/
  Serial.println("Connecting to WiFi");
  /*thing.add_wifi("your_wifi_ssid", "your_wifi_ssid_password");*/
  thing.add_wifi(SECRET_SSID, SECRET_PASS);
  Serial.println("WiFi Connected");
  
  thing["Temperature"] >> [](pson& out){
      out = temperatureInC();
  };

  /*The relay state is obtained as an input resource (integer/ number) from thinger.io*/
  thing["RelayState"] << [](pson& in){
      relayState = in["state"];
      changeRelayState();
  };

  /*Connecting to Wi-Fi - IFTTT*/
  while (!Serial) {
    ; /*Wait for serial port to connect. Needed for native USB port only*/
  }
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
    Serial.println(ssid);
    /*Connect to WPA/WPA2 network. Change this line if using open or WEP network:*/
    status = WiFi.begin(ssid, pass);
    /*wait 10 seconds for connection:*/
    delay(10000);
  }
  Serial.println("Connected to wifi");
  printWiFiStatus();
  delay(5);
  digitalWrite(statusLED, LOW);
  digitalWrite(collisionLED, LOW);
}

/*************************************Loop*************************************/
void loop() {
  unsigned long currentMillis = millis();
  unsigned long currentMillis1 = millis();

  /*if (currentMillis - previousMillis >= tempFeedDelay){
    
    previousMillis = currentMillis;
  }*/
  thing.handle();
  
  Serial.println(temperatureInC());

  XValue = analogRead(XAxis);
  YValue = analogRead (YAxis);
  detectFall (XValue, YValue);
  digitalWrite(collisionLED, LOW);
  
  measDistanceF = calculateDistance(trigPinF, echoPinF);
  /*For Debugging purpose*/
  Serial.print(measDistanceF);
  Serial.print('\t');
  measDistanceR = calculateDistance(trigPinR, echoPinR);
  Serial.println(measDistanceR);

  if ((measDistanceF <= 10) || (measDistanceR <= 10)){
    Serial.println("Collision Alert!");
    if (currentMillis1 - previousMillis1 >= collisionDelay){
      digitalWrite(collisionLED, HIGH);
      previousMillis1 = currentMillis1;
    }
  }
  else if ((measDistanceF <= 30) || (measDistanceR <= 30)){
    digitalWrite(collisionLED, HIGH);
  }
  else
  {
    digitalWrite(collisionLED, LOW);
  }
  
}


/**************************User Defined Functions******************************
 * float temperatureInC()                           - To calculate temperature in degrees Celsius
 * void detectFall (int X, int Y)                   - To detect a fall
 * int calculateDistance(int trigPin, int echoPin)  - To calculate distance
 * void printWiFiStatus()                           - To print WiFi Status
 * void sendSMS()                                   - To send SMS 
 * void changeRelayState()                          - To change the relay state on the basis of input received from thinger.io
 */

float temperatureInC(){
  sensorVoltage = analogRead(tempSensor);
  sensorVoltage = (sensorVoltage * 3.3)/ 1024.0;
  tempinC = (sensorVoltage * 0.28828125) * 100.0;
  return tempinC;
}


void detectFall (int X, int Y){
  /*Code to detect fall, The values below are assessed keeping the center position around the value 530 from X & Y axes*/
  if ( X < 430 or X > 650 ){ XFlag = 1; }
  else { XFlag = 2; }

  if ( Y < 430 or Y > 650 ){ YFlag = 1; }
  else { YFlag = 2; }

  /* For Debugging purpose */
  Serial.print(XFlag);Serial.print('\t');
  Serial.print(YFlag);Serial.print('\t');
  Serial.print(X);Serial.print('\t');
  Serial.print(Y);Serial.print('\t');
  
  if (XFlag == 1 or YFlag == 1) {
    fallCount = fallCount + 1;
    Serial.print("Fall Detected! C: ");
    Serial.println(fallCount);
    digitalWrite(collisionLED, HIGH);
    delay(1000);

    if(fallCount >= 10){
      sendSMS();
      fallCount = 0;
      
      while(true);
    }
  }
}

int calculateDistance(int trigPin, int echoPin)
{
  /*Code to calculate Distance from Duration*/
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  duration = pulseIn(echoPin, HIGH);
  distance= duration*0.034/2;
  return distance;
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

void sendSMS(){
  Serial.println("\nStarting connection to server...");
  /*If you get a connection, report back via serial:*/
  if (client.connect(server, 80)) {
    Serial.println("connected to server");
    /*Make a HTTP request:
    Find instructions to obtain the below value here - https://www.element14.com/community/community/design-challenges/designforacause/blog/2018/08/16/cue-system-for-anosmia-and-smart-wheelchair-5-fall-and-collision-detection#jive_content_id_Obtaining_the_Webhook_URL_to_trigger_an_event_from_Arduino_MKR1000  */
    client.println("GET /trigger/{YOUR_IFTTT_EVENT_NAME}/with/key/{YOUR_KEY} HTTP/1.1");
    client.println("Host: maker.ifttt.com");
    
    client.println("Connection: close");
    client.println();
    digitalWrite(statusLED, HIGH);
  }

  int reqReceived = 1;

  while(reqReceived){
  
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
      reqReceived = 0;
    }
  
    /*If the server's disconnected, stop the client:*/
    if (!client.connected()) {
      Serial.println();
      Serial.println("disconnecting from server.");
      client.stop();
      digitalWrite(statusLED, LOW);
    }
  }
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
