/* This is the code for the 'Cue System for Anosmia' part of my project deisgned for the Element14 Community's 'Design for a Cause' design challenge, 
 *  sponsored by Arduino. In particular, this is for the extra smell sensing unit with Bluetooth Slave Arduino UNO.
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
 * Hardware components Required (Main Unit)
 * ----------------------------------------
 * 1. Arduino UNO board
 * 2. Gas Sensors (MQ-2 & MQ-5)
 * 3. Four Channel Relay Module
 * 4. Bluetooth module
 * 5. ProtoShield/ Breadboard and Few Jumper wires
 * 6. 12V Power Adapter
 * 7. DC Fan
 * 
 * Connections
 * -----------
 * Arduino UNO      |   Relay Module
 * ---------------------------------------------
 *      5V          |         VCC
 *      Gnd         |         Gnd
 *      IN1         |         D10     
 *      IN2         |         D11
 *      IN3         |         D5
 *      IN4         |         D6
 * Arduino UNO      |   Bluetooth Module
 * ---------------------------------------------
 *      5V          |         +5V
 *      Gnd         |         Gnd
 *      RX          |         RX     
 *      TX          |         TX
 * Arduino UNO      |   Gas Sensors(MQ-2/ MQ-5)
 * ---------------------------------------------
 *      5V          |         VCC
 *      Gnd         |         Gnd
 *      A0/ A1      |         A0     
 *      -           |         D0
 *      
 */

/*Declaration and initialisation of Variables*/
int mq2 = A0;
int mq5 = A1;
int Relay1 = 10;
int Relay2 = 11;
int Relay3 = 5;
int Relay4 = 6;
int serialData = 0;

void setup() {
  /*Assigning GPIO pin modes*/
  pinMode(mq2, INPUT); 
  pinMode(mq5, INPUT);
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);

  /*Starting the Serial Communication*/
  Serial.begin(38400);

  /*Setting the Relay Pin to LOW State*/
  digitalWrite(Relay1, HIGH);
  digitalWrite(Relay2, HIGH);
  digitalWrite(Relay3, HIGH);
  digitalWrite(Relay4, LOW);
}

void loop() {
  /*Send the Sensor values to Master/ Serial Port*/
  sendGasSensorValue();
  relayStateMonitor();
}

void sendGasSensorValue(){
  /*Send data from gas sensors as comma separated values to Arduino MKR1000 Serially (BT)*/
  Serial.print(analogRead(mq2));
  Serial.print(',');
  Serial.print(analogRead(mq5));
  Serial.print(',');
  Serial.println();
}

void relayStateMonitor (){
  /*Listen to the master Arduino MKR1000 and turn ON or OFF the respective relay*/
  if(Serial.available()>0){
     serialData = Serial.read();
     Serial.println(serialData);
     if (serialData == 49){
      digitalWrite(Relay1, LOW);
     }
     else if (serialData == 50){
      digitalWrite(Relay1, HIGH);
     }
     else if (serialData == 51){
      digitalWrite(Relay2, LOW);  
     }
     else if (serialData == 52){
      digitalWrite(Relay2, HIGH); 
     }
     else if (serialData == 53){
      digitalWrite(Relay3, LOW);
     }
     else if (serialData == 54){
      digitalWrite(Relay3, HIGH);
     }
     else if (serialData == 55){
      digitalWrite(Relay4, LOW);  
     }
     else if (serialData == 56){
      digitalWrite(Relay4, HIGH); 
     }
  } 
}
