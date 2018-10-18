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
  int sensorValue = analogRead(A5);// read the input on analog pin 0:
  float voltage = sensorValue * (5.0 / 1024.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  //float valeur = voltage*0.1035 - 0.292;
  float valeur = -1120.4*voltage*voltage + 5742.3*voltage - 4352.9;
  Serial.print(voltage);
  Serial.print(" V => ");
  Serial.print(valeur); // print out the value you read:
  Serial.println(" NTU");
  delay(500);
}


