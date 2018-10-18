long pause=0;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pause = millis();
  
  attachInterrupt(0, updatePluvio, RISING); //pin 3
}

void loop() {
  // put your main code here, to run repeatedly:
if(millis() - pause > 100)
  {
    Serial.println(nbBasculement);//delay(500);
    nbBasculement=0;
    pause = millis();
  }
  //t.update();
  delay(10);
}

void updateBasculement(){
  nbBasculement++;
}

