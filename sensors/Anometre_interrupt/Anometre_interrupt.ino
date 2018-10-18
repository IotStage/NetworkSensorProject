uint8_t change=0;
int nbTour=0;
void setup() {
Serial.begin(9600);
pinMode(3, INPUT);
attachInterrupt(1, getTour, CHANGE);
}

void loop() {
  // put your main code here, to run repeatedly:
  int val = digitalRead(3);
   //float val2 = analogRead(A0)*(5.0/1023.0);
Serial.print(nbTour);
Serial.print(",");
//Serial.print(change);
//Serial.print(",");
Serial.println(val);
nbTour=0;
delay(5);

}

void getTour(){
  //Serial.println(digitalRead(3));
  change++;
  if(change%8 == 0){
    nbTour++;
    change=0;
  }
}

