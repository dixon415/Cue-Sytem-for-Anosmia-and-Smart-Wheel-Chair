/* This is the code for the 'Home Appliance Control' section of the Smart WheelChair deisgned for the Element14 Community's 'Design for a Cause' design challenge, 
 *  sponsored by Arduino. In particular, this is for the main smell sensing unit with Bluetooth Master Arduino MKR1000.
 * 
 * Design Challenge Page -
 * https://www.element14.com/community/community/design-challenges/designforacause/blog
 * 
 * Author : Dixon Selvan
 * Date   : August 31, 2018
 * Project: Cue system for Anosmia and Smart WheelChair
 * Website: 
 * 
 * Hardware components Required (Main Unit)
 * ----------------------------------------
 * 1. Arduino MKR1000 board
 * 2. Bluetooth module
 * 3. ProtoShield/ Breadboard and Few Jumper wires
 * 
 * Connections
 * -----------
 * Arduino UNO      |   Bluetooth Module
 * ---------------------------------------------
 *      5V          |         +5V
 *      Gnd         |         Gnd
 *      RX          |         RX     
 *      TX          |         TX
 * 
 * Blog Link -
 * 
 */
 
void setup() {
  Serial1.begin(38400);
}

void loop() {
  for(int i=1; i<=8;i++){
    passCmdCode(i);  
  }
}

void passCmdCode (int code){
  Serial1.print(code);
  delay(1000);
}

