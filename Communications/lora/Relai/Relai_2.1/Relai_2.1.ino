
#include <SPI.h>
#include <RH_RF95.h>
#include <String.h>
#include "encrypt.h"


RH_RF95 rf95;
ENCRYPT encrypt_decrypt;

//Define the timeout to re-start to listen the broadcast info from server to establish network.
//Default: 10 minutes 
#define CHECK 100

//Define the LoRa frequency use for this client
float frequency = 868.0;

//Define the encrypt encryptkey. so different group of LoRa devices won't communicate with each other.
unsigned char encryptkey[16]={0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF,0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF};

// Client ID address in EEPROM.
#define ID_ADDRESS 0x00


int sent_count = 0;//Client send count, increase after sent data. 
int client_id = 0;
long temps = millis();


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
    if (!rf95.init())
      Serial.println("init failed");
      
    //Setup ISM frequency
    rf95.setFrequency(frequency);
    // Setup Power,dBm
    rf95.setTxPower(13);
   // client_id = EEPROM.read(ID_ADDRESS);//Get Client id. 
  Serial.println("debut");
}

void loop() {
  if(temps - millis() > CHECK){
    listen_msg();
    temps = millis();
  }

}



void listen_msg(void){
  if (rf95.waitAvailableTimeout(CHECK))
  {
    Serial.println("Reception d'un message");
    char buf[RH_RF95_MAX_MESSAGE_LEN];//buffer to store the server response message
    uint8_t len = sizeof(buf);// data length
    if (rf95.recv(buf, &len))//check if receive data is correct 
    {    
      Serial.println("decryptage du message "); 
      Serial.println(buf);         
      encrypt_decrypt.btea(buf, -len, encryptkey);
      Serial.println(buf);
      Serial.print(buf[0]);Serial.print(buf[1]);Serial.print(buf[2]);Serial.println(buf[3]);
      if(buf[0] == 'I' && buf[1] == 'N' && buf[2] == 'F' && buf[3] == ':' ){
        encrypt_decrypt.btea(buf, len, encryptkey);
        rf95.send(buf,len);// Send a Join Message
        rf95.waitPacketSent();// wait for send finished 
      }else if(buf[0] == 'S' && buf[1] == 'U' && buf[2] == 'P' && buf[3] == ':' )
      {
        String valeur = String(random(0, 25));
        int len = valeur.length()+1;
        char buff[len];
        //Serial.println(paquet);
        valeur.toCharArray(buff, len);
        char buff2[len+4]; 
        buff2[0] = 'C';
        buff2[1] = 'T';
        buff2[2] = 'L';
        buff2[3] = ':';
        for(int i=4; i<len+4; i++){
          buff2[i] = buff[i-4];
        }
        encrypt_decrypt.btea(buff2, len+4, encryptkey);
        rf95.send(buff2, len+4);
        Serial.print("trame de reponse ");
        Serial.println(buff2);
        //reception d'un packet de supervision   
      }else if(buf[0] == 'C' && buf[1] == 'M' && buf[2] == 'D' && buf[3] == ':' )
      {
        Serial.print("transfert du message :");
        //reception d'un paquet de commande
        Serial.println(buf);
        encrypt_decrypt.btea(buf, len, encryptkey);
        Serial.println(buf);
        rf95.send(buf,len);// Send the cmd info Message
        rf95.waitPacketSent();// wait for send finished 
      }
    }
  }
}

