#include <Wire.h>
char x = '0';
long debutTransmission;
long finTransmission;
void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);
  // put your setup code here, to run once:
debutTransmission = millis();
finTransmission = 1000;
}

void loop() {
  // put your main code here, to run repeatedly:
if(Serial.available())
   //sendToSlave(Serial.readString());
//else
  sendToXSlave();
getSlaveMessage();
delay(1000);
}

void getSlaveMessage(){
  String msg="";
  long tmp = millis();
  if(tmp - debutTransmission > finTransmission){
    Wire.requestFrom(8, 18);    // request 6 bytes from slave device #8
    while (Wire.available()) { // slave may send less than requested
      char c = Wire.read(); // receive a byte as character
      msg+=c;         // print the character
     }
      Serial.println(msg);
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
  String value = Serial.readString();
  String mode = value.substring(0,4);//.toUpperCase();
  String v = value.substring(5,6);
  if(mode == "MODE" )
   {
    int c = v.toInt();
    if(c < 5)
    {
      Serial.print("set mode to ");
      Serial.println(c);
      switch(c){
        case 0:sendElement('0');break;
        case 1:sendElement('1');break;
        case 2:sendElement('2');break;
        case 3:sendElement('3');break;
        case 4:sendElement('4');break;
      }
      
    }else Serial.println("erreur lors du transfert du mode ");
   }else Serial.println("erreur du mode entre");
    
  
  //else x++;
}

void sendElement(char e){
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(e);              // sends one byte
  Wire.endTransmission();
}

