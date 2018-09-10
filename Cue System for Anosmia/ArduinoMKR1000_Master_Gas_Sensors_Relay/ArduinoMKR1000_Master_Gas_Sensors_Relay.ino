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

#include <WiFi101.h>
#include <ThingerWifi101.h>
#include "arduino_secrets.h"

/*Declaring Variables*/
int relayState = 0;

/*Create an account in thinger.io and replace username below with that username. 
Create a new device and replace deviceId, deviceCredential below with the one you had created.*/
ThingerWifi101 thing(SECRET_USERNAME, SECRET_DEVICEID, SECRET_DEVICECREDENTIAL);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
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
}

void loop() {
  thing.handle();
}

void changeRelayState(){
  if(relayState == 1)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial1.print(1);
  }
  else if (relayState == 2){
    digitalWrite(LED_BUILTIN, LOW);
    Serial1.print(2);
  }
  else if(relayState == 3)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial1.print(3);
  }
  else if (relayState == 4){
    digitalWrite(LED_BUILTIN, LOW);
    Serial1.print(4);
  }
  else  if(relayState == 5)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial1.print(5);
  }
  else if (relayState == 6){
    digitalWrite(LED_BUILTIN, LOW);
    Serial1.print(6);
  }
}
