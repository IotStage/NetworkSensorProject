/*
# Example code d'utilisation ORP meter V1.0.
 # Editeur : bngesp
 # Date   : 2017.03.16
 # Product: Turbidity
 # SKU    : SEN0189
*/

void setup() {
  Serial.begin(9600); //Baud rate: 9600
}
void loop() {
  int sensorValue = analogRead(A2);// read the input on analog pin 0:
  float voltage = sensorValue * (5.0 / 1023.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float value = 150.0*voltage - 80;
   Serial.print(voltage);
   Serial.print(" V => ");
  Serial.print(value); // print out the value you read:
   Serial.println(" NTU");
  delay(500);
}


