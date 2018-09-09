int state = 0;

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(38400);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial1.available() > 0){
    state = Serial1.read();    
    Serial1.println(state);
  }

  Serial1.write(11);
  delay(500);
  Serial1.write(21);
  delay(500);
  Serial1.write(10);
  delay(500);
  Serial1.write(20);
  delay(500);
}
