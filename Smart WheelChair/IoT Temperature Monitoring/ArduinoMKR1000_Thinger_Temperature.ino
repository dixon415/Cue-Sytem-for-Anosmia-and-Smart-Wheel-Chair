/* This is the code for the IoT Temperature measurement and monitoring section of the Smart WheelChair deisgned for the Element14 Community's 'Design for a Cause' 
 * design challenge, sponsored by Arduino.
 * 
 * Author : Dixon Selvan
 * Date   : July 28, 2018
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

/*Disabling the secure TLS/SSL connection*/
#define _DISABLE_TLS_

#include <WiFi101.h>
#include <ThingerWifi101.h>

/*Declaring Variables*/
int tempSensor = A1;
float sensorVoltage = 0.0;
float tempinC = 0.0;
float tempinF = 0.0;

/*Create an account in thinger.io and replace username below with that username. 
Create a new device and replace deviceId, deviceCredential below with the one you had created.*/
ThingerWifi101 thing("username", "deviceId", "deviceCredential");

void setup() {
  /*Replace the below accrodingly with your WiFi SSID and password*/
  thing.add_wifi("your_wifi_ssid", "your_wifi_ssid_password");
  
  pinMode(tempSensor, INPUT);
  /*The analog read temperature data (converted to Celsius) is passed as an output resource to thinger.io*/
  thing["Temperature"] >> [](pson& out){
      out = temperatureInC();
  };
}

void loop() {
  thing.handle();
}

float temperatureInC(){
  /*Reading analog voltage from the temperature sensor and converting it to Celsius*/
  sensorVoltage = analogRead(tempSensor);
  sensorVoltage = (sensorVoltage * 3.3)/ 1024.0;
  tempinC = (sensorVoltage - 0.5) * 100.0;
  delay(100);
  return tempinC;
}

