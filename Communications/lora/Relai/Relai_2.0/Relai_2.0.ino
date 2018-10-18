#include <RH_RF95.h>
#include <SPI.h>

#define ADDRESSE_NODE 9
#define ADDRESSE_GATEWAY 4 
#define ADDRESSE_SENDER 5

int e=0;
String paquet="";
int DELAY = 20; //2 secondes
long temps=0;

RH_RF95 rf95;
float frequency = 868.0;

void setup() {
  Serial.begin(38400);
  initRF();
  temps = millis();
  
}

void loop() {

  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
  
  if (rf95.waitAvailableTimeout(10000))
  { 
    // Should be a reply message for us now   
    if (rf95.recv(buf, &len))
   {
      Serial.print("got reply: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC); 
      rf95.send(buf, sizeof(buf));
    }
    else
    {
      Serial.println("recv failed");
    }
  }

  
}


void initRF(){
  Serial.println("Debut initialisation RF");

  if (!rf95.init())
    Serial.println("init failed");
   // Setup ISM frequency
  rf95.setFrequency(frequency);
  // Setup Power,dBm
  rf95.setTxPower(13);
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  Serial.print("Configuration terminee");
}

/*boolean availablePacket(){
   e = sx1276.setChannel(CH_10_868);
  e = sx1276.receivePacketTimeout(10000);
  //e = sx1276.getRSSIpacket();
  paquet = sx1276.getPacketRecu();
  Serial.print(("Receive packet timeout, state "));
  Serial.println(e, DEC);
  
  if(e == 0 && sx1276._payloadlength > 0){
    /*for(int i=0; i<sx1276._payloadlength; i++){
      Serial.print((char)sx1276.packet_received.data[i]);
    }
    //Serial.print("un paquet recu");
    //Serial.println(paquet);
    Serial.println();
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
   e = sx1276.setChannel(CH_11_868); 
  e = sx1276.sendPacketTimeout(idClient, buff);
  Serial.print("Packet sent, state ");
  Serial.println(e, DEC);
  Serial.println(buff);
  }else Serial.println("echec envoi");
  
  
}
*/
