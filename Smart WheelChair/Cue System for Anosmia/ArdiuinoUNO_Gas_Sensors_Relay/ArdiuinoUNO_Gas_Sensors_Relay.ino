/*Declaration and initialisation of Variables*/
int mq2 = A0, mq5 = A1;
int sensorVal = 0;
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
  Serial.print(analogRead(mq2));
  Serial.print(',');
  Serial.print(analogRead(mq5));
  Serial.print(',');
  Serial.println();
}

void relayStateMonitor (){
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
