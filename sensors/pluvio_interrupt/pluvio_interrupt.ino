volatile int nb=0;
// 1 basculement = 0.27 mm
void setup() {
  Serial.begin(9600);
  attachInterrupt(1, basculement, RISING);
}

void loop() {
 
Serial.println(nb/5);
delay(100);
}

void basculement(){
  nb++;
}

