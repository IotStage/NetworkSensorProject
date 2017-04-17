void setup() {
Serial.begin(9600);
pinMode(2, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int val = digitalRead(2);
Serial.println(val);
//Serial.println(" V");
delay(5);


}
