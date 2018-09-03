/* This is the code for the Home Appliance Control section of the Smart WheelChair deisgned for the Element14 Community's 'Design for a Cause' 
 * design challenge, sponsored by Arduino.
 * 
 * Author : Dixon Selvan
 * Date   : September 03, 2018
 * Project: Cue system for Anosmia and Smart WheelChair
 * Website: 
 *
 * Blog Link - 
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
#include "arduino_secrets.h"

/*Declaring Variables*/
int relayState = 0;

/*Create an account in thinger.io and replace username below with that username. 
Create a new device and replace deviceId, deviceCredential below with the one you had created.*/
ThingerWifi101 thing(SECRET_USERNAME, SECRET_DEVICEID, SECRET_DEVICECREDENTIAL);

void setup() {
  /*Replace the below accrodingly with your WiFi SSID and password*/
  pinMode(LED_BUILTIN, OUTPUT);
  
  thing.add_wifi(SECRET_SSID, SECRET_PASS);

  thing["Test"] << [](pson& in){
      relayState = in["state"];
      changeRelayState();
  };
}

void loop() {
  thing.handle();
}

void changeRelayState(){
  if(relayState == 1)
  {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else if (relayState == 2){
    digitalWrite(LED_BUILTIN, LOW);
  }
}
