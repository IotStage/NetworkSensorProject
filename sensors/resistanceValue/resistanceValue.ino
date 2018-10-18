
void setup() {
Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  float val = analogRead(A0)*(5.0/1023.0);
Serial.print(val);
Serial.println(" V");
delay(10);


}
