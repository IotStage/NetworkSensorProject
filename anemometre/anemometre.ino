void setup() {
Serial.begin(9600);
pinMode(3, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int val = digitalRead(3);
   //float val2 = analogRead(A0)*(5.0/1023.0);
Serial.println(val);
//Serial.print(", ");
//Serial.println(val2);
delay(5);


}
