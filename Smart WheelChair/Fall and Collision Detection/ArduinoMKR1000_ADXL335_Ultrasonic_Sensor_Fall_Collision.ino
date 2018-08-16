/* This is the code for the Fall and Collision Detection section of the Smart WheelChair deisgned for the Element14 Community's 'Design for a Cause' 
 * design challenge, sponsored by Arduino.
 * 
 * This code contains extracts from the WiFi101 WebClient example code, 'created 13 July 2010 by dlf (Metodo2 srl) modified 31 May 2012 by Tom Igoe'
 * 
 * Author : Dixon Selvan
 * Date   : August 14, 2018
 * Project: Cue system for Anosmia and Smart WheelChair
 * Website: 
 * 
 * Hardware components Required
 * ----------------------------
 * 1. Arduino MKR1000 board
 * 2. HC-SR04 Ultrasonic Sensors - x2
 * 3. Accelerometer ADXL335Z Evaluation Board (any ADXL XXX should work)
 * 4. ProtoShield/ Breadboard and Few Jumper wires
 * 
 * Connections
 * -----------
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
 *      D1          |   Positive of Green LED      
 *      D2          |   Positive of Red LED
 *      Gnd         |   Negative of both the LEDs
 *
 * Blog Link - https://www.element14.com/community/community/design-challenges/designforacause/blog/2018/08/03/cue-system-for-anosmia-and-smart-wheelchair-5-fall-and-collision-detection
 * 
 * Design Challenge Page -
 * https://www.element14.com/community/community/design-challenges/designforacause/blog
 * 
 * Create a new applet using IFTTT - https://ifttt.com/create
 */

/*Adding Required Libraries*/
#include <SPI.h>
#include <WiFi101.h>
#include "arduino_secrets.h" 

/*Please enter your sensitive data and credential in the Secret tab/arduino_secrets.h*/
char ssid[] = SECRET_SSID;  // Your network SSID (name)
char pass[] = SECRET_PASS;  // Your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;           // Your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
/*Find instructions to obtain the below value here - https://www.element14.com/community/community/design-challenges/designforacause/blog/2018/08/03/cue-system-for-anosmia-and-smart-wheelchair-5-fall-and-collision-detection#jive_content_id_Obtaining_the_Webhook_URL_to_trigger_an_event"*/
char server[] = "maker.ifttt.com"; 

/* Initialize the Ethernet client library with the IP address and port of the server that you want to connect to (port 80 is default for HTTP):*/
WiFiClient client;

/*Declaring and initializing variables*/
int trigPinF = 5, trigPinR = 4;
int echoPinF = 2, echoPinR = 3;
int collisionLED = 1;     //Red LED to Alert user in case of collision
int statusLED = 0;        //Green LED for status

int XAxis = A3, YAxis = A2;

int XValue = 0, YValue = 0;
int XFlag = 0, YFlag = 0;
int fallCount = 0;

long duration;
int distance;
int measDistanceF;
int measDistanceR;

void setup() {
  /*Declaring GPIO pins connection mode I/O*/
  pinMode(trigPinF, OUTPUT);
  pinMode(echoPinF, OUTPUT);
  pinMode(trigPinR, OUTPUT);
  pinMode(echoPinR, OUTPUT);
  pinMode(collisionLED, OUTPUT);
  pinMode(statusLED, OUTPUT);

  pinMode (XAxis, INPUT);
  pinMode (YAxis, INPUT);

  /*Serial initialisation for debugging*/
  Serial.begin(9600);
  
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
    digitalWrite(statusLED, HIGH);
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

void loop() {
  
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
    digitalWrite(collisionLED, HIGH);
    delay(1);
  }
  else
  {
    digitalWrite(collisionLED, LOW);
    delay(1);
  }
}


/* User-Defined Functions:
 * void detectFall (int X, int Y)                   - To detect a fall
 * int calculateDistance(int trigPin, int echoPin)  - To calculate distance
 * void printWiFiStatus()                           - To print WiFi Status
 * void sendSMS()                                   - To send SMS
 */

void detectFall (int X, int Y){
  /*Code to detect fall, The values below are assessed keeping the center position around the value 530 from X & Y axes*/
  if ( X < 430 or X > 600 ){ XFlag = 1; }
  else { XFlag = 2; }

  if ( Y < 430 or Y > 600 ){ YFlag = 1; }
  else { YFlag = 2; }

  /* For Debugging purpose
  Serial.print(XFlag);Serial.print('\t');
  Serial.print(YFlag);Serial.print('\t');
  Serial.print(X);Serial.print('\t');
  Serial.print(Y);Serial.print('\t');*/

  delay(10);
  
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
    Find instructions to obtain the below value here - https://www.element14.com/community/community/design-challenges/designforacause/blog/2018/08/03/cue-system-for-anosmia-and-smart-wheelchair-5-fall-and-collision-detection#jive_content_id_Obtaining_the_Webhook_URL_to_trigger_an_event"*/
    client.println("GET /trigger/FallDetection/with/key/9DAktc7DOFMjcP7sS_4CP HTTP/1.1");
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

