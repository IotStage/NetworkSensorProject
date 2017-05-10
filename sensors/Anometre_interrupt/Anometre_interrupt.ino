volatile uint8_t change=0;
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
nbTour=0;
Serial.print(", ");
Serial.println(val);
delay(50);

}

void getTour(){
  change++;
  if(change%4 == 0){
    nbTour++;
    change=0;
  }
}

