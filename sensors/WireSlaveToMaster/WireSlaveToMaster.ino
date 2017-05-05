#include <Wire.h>
char mode='a';

void setup() {
  Serial.begin(9600);
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(envoiEvent); // register event
  Wire.onReceive(recuEvent);
}

void loop() {
 delay(100);
}
void envoiEvent() {
  switch(mode){
    case '1': Wire.write("mode 1 : send temp");break; //18 bytes
    case '2': Wire.write("mode 2 : send ORP ");break;
    case '3': Wire.write("mode 3 : send EC  ");break;
    case '4': Wire.write("mode 4 : send PH  ");break;
    default: Wire.write("-> temp,ORP,EC,PH ");
  }
  Serial.print("Mode =  ");
  Serial.println(mode);
  //Wire.write("bonjour master");
}

void recuEvent(){
  //Serial.println("donnees recu");
  while (Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    mode = c;
    Serial.print(c);         // print the character
  }
  int x = Wire.read();    // receive byte as an integer
  Serial.print(" Mode change to ");
  Serial.println(mode); 
}



