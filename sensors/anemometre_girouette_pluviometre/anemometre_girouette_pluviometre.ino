

void setup() {
Serial.begin(9600);
pinMode(5, INPUT);
pinMode(3, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
int val = digitalRead(5);
float val2 = analogRead(A0)*(5.0/1023.0);
int val3 = digitalRead(3);
Serial.print(val*5);
Serial.print(",");
Serial.print(val2);
Serial.print(",");
Serial.println(val3);
delay(5);


}
