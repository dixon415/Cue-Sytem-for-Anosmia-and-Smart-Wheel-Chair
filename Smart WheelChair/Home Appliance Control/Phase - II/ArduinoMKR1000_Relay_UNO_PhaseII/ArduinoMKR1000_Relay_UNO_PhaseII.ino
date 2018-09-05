/* This is the code for the 'Home Appliance Control' section of the Smart WheelChair deisgned for the Element14 Community's 'Design for a Cause' design challenge, 
 *  sponsored by Arduino. In particular, this is for the extra smell sensing unit with Bluetooth Slave Arduino UNO.
 * 
 * Design Challenge Page - https://www.element14.com/community/community/design-challenges/designforacause/blog
 * 
 * Blog Link - 
 * 
 * Author : Dixon Selvan
 * Date   : September 04, 2018
 * Project: Cue system for Anosmia and Smart WheelChair
 * Website: 
 * 
 * Hardware components Required (Main Unit)
 * ----------------------------------------
 * 1. Arduino UNO board
 * 2. Four Channel Relay Module
 * 3. Bluetooth module
 * 4. ProtoShield/ Breadboard and Few Jumper wires
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
 * 
 */
 
int Relay1 = 10;
int Relay2 = 11;
int Relay3 = 5;
int Relay4 = 6;
int serialData = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);
  Serial.begin(38400);
  digitalWrite(Relay1, HIGH);
  digitalWrite(Relay2, HIGH);
  digitalWrite(Relay3, HIGH);
  digitalWrite(Relay4, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
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
