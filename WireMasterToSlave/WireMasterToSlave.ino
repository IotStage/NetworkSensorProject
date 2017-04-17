#include <Wire.h>
int x = 0;
long debutTransmission;
long finTransmission;
void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);
  // put your setup code here, to run once:
debutTransmission = millis();
finTransmission = 10000;
}

void loop() {
  // put your main code here, to run repeatedly:
if(Serial.available())
   sendToSlave(Serial.readString());
else
  sendToXSlave();
getSlaveMessage();
delay(1000);
}

void getSlaveMessage(){
  long tmp = millis();
  if(tmp - debutTransmission > finTransmission){
    Wire.requestFrom(8, 18);    // request 6 bytes from slave device #8
    while (Wire.available()) { // slave may send less than requested
      char c = Wire.read(); // receive a byte as character
      Serial.print(c);         // print the character
     }
      Serial.println();
      debutTransmission = tmp;
  }
 
}

void sendToSlave(String msg){
  int len = msg.length();
  byte buff[len];
  msg.toCharArray(buff, len);
  Wire.beginTransmission(8); // transmit to device #8
  //Wire.write("x is "); // sends five bytes
  Wire.write(buff, len);              // sends one byte
  Wire.endTransmission();    // stop transmitting

  //x++;
}

void sendToXSlave(){
  Serial.println(x);
  Wire.beginTransmission(8); // transmit to device #8
  //Wire.write("x is ");        // sends five bytes
  Wire.write("0");              // sends one byte
  Wire.endTransmission();    // stop transmitting
  //x= x == 5 ? x++ : 0;
  if(x == 4) x = 0;
  else x++;
}

