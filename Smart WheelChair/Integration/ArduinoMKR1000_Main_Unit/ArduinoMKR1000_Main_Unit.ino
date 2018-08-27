/* This is the integrated code for the 'IoT Temperature measurement and monitoring', 'Fall and Collision Detection' and 'Home Appliance Control' sections of the 
 * Smart WheelChair deisgned for the Element14 Community's 'Design for a Cause' design challenge, sponsored by Arduino.
 * 
 * Author : Dixon Selvan
 * Date   : August 31, 2018
 * Project: Cue system for Anosmia and Smart WheelChair
 * Website: 
 * 
 * Hardware components Required
 * ----------------------------
 * 1. Arduino MKR1000 board
 * 2. TMP36 temperature sensor
 * 3. Few Jumper wires
 * 
 * Connections
 * -----------
 * Arduino MKR1000  |   TMP36 Temperature Sensor
 * ---------------------------------------------
 *      3.3V        |         +Vs
 *      Gnd         |         Gnd
 *      A1          |         Vout
 *
 * Blog Link - https://www.element14.com/community/community/design-challenges/designforacause/blog/2018/07/28/cue-system-for-anosmia-and-smart-wheelchair-4-iot-temperature-monitoring-diy
 * 
 * Design Challenge Page -
 * https://www.element14.com/community/community/design-challenges/designforacause/blog
 * 
 * Connecting Arduino MKR1000 with thinger.io, getting started guide - http://docs.thinger.io/arduino/
 */

#define _DISABLE_TLS_

/*Adding Required Libraries*/
#include <SPI.h>
#include <WiFi101.h>
#include <ThingerWifi101.h>
#include "arduino_secrets.h"

/*Please enter your sensitive data and credential in the Secret tab/arduino_secrets.h*/
char ssid[] = SECRET_SSID;  // Your network SSID (name)
char pass[] = SECRET_PASS;  // Your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;           // Your network key Index number (needed only for WEP)

int tempSensor = A1;
int statusLED = 1;
int collisionLED = 0;
float sensorVoltage = 0.0;
float tempinC = 0.0;
float tempinF = 0.0;

unsigned long previousMillis = 0;
long tempDelay = 100;

//ThingerWifi101 thing("username", "deviceId", "deviceCredential");
ThingerWifi101 thing(SECRET_USERNAME, "ArduinoMKR1000", SECRET_DEVICECREDENTIAL);

void setup() {
  //thing.add_wifi("your_wifi_ssid", "your_wifi_ssid_password");
  pinMode(statusLED, OUTPUT);
  pinMode(collisionLED, OUTPUT);
  pinMode(tempSensor, INPUT);
  
  Serial.begin(38400);
  
  Serial.println("Connecting to WiFi");
  digitalWrite(collisionLED, HIGH);
  thing.add_wifi(SECRET_SSID, SECRET_PASS);
  Serial.println("WiFi Connected");
  digitalWrite(statusLED, HIGH);
  
  thing["Temperature"] >> [](pson& out){
      out = temperatureInC();
  };
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= tempDelay){
    thing.handle();
    previousMillis = currentMillis;
  }
  Serial.println(temperatureInC());
}

float temperatureInC(){
  sensorVoltage = analogRead(tempSensor);
  sensorVoltage = (sensorVoltage * 3.3)/ 1024.0;
  tempinC = (sensorVoltage * 0.28828125) * 100.0;
  return tempinC;
}

