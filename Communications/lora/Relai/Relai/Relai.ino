#include <SX1276.h>
#include <SPI.h>

#define ADDRESSE_NODE 7
#define ADDRESSE_GATEWAY 9 
#define ADDRESSE_SENDER 3

int e=0;
String paquet="";
int DELAY = 2000; //2 secondes


void setup() {
  Serial.begin(115200);
  initRF();
  
}

void loop() {
  if(availablePacket())
     sendPaquet(sx1276.getPacketRecu(), ADDRESSE_GATEWAY);
  delay(DELAY);

}


void initRF(){
  Serial.println("Debut initialisation RF");

  sx1276.ON();
  e = sx1276.setMode(1);
  Serial.print("configuration du mode de transmission ");
  Serial.println(e, DEC);
  
  e = sx1276.setChannel(CH_16_868);
 Serial.print("configuration du canal de transmission ");
  Serial.println(e, DEC);
  
  e = sx1276.setPower('M');
  Serial.print("configuration de la puissance ");
  Serial.println(e);
  
  
  e = sx1276.setNodeAddress(ADDRESSE_NODE);
  Serial.print("configuration de l'adresse du relai ");
  Serial.println(e, DEC);
  
  Serial.print("Configuration terminee");
}

boolean availablePacket(){
  e = sx1276.receivePacketTimeout(10000);
  //e = sx1276.getRSSIpacket();
  paquet = sx1276.getPacketRecu();
  //Serial.print(("Receive packet timeout, state "));
  //Serial.println(e, DEC);
  
  if(e == 0 && sx1276._payloadlength > 0){
    Serial.print("un paquet recu");
    //Serial.println(paquet);
   return true;
  }else{
    Serial.println("pas de nouveau paquet");
  }
  return false;
}

void sendPaquet(String paquet, int idClient){
  if(paquet.length() > 0){
    char buff[paquet.length()+1];
  paquet.toCharArray(buff, paquet.length()+1); 
  e = sx1276.sendPacketTimeout(idClient, buff);
  Serial.print("Packet sent, state ");
  Serial.println(e, DEC);
  Serial.println(buff);
  }else Serial.println("echec envoi");
  
  
}

