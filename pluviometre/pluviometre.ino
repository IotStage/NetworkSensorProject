const int pinPluvio = 3;
int nbBasculement=0;
int avant=1;
int value=0;

void setup() {
  Serial.begin(9600);
  pinMode(pinPluvio, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  getBasculement();
  Serial.println(nbBasculement);
}

void getBasculement(){
   value = digitalRead(pinPluvio);
  //tension_pluvio=(sensorValue_pluvio*5)/1023;
  Serial.print(value);
  Serial.print(",");
  if(value==0 && avant==1){
    nbBasculement++;
  }
  avant=value;
  //return nbtour;
}

