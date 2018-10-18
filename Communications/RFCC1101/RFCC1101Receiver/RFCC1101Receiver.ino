#include "EEPROM.h"
#include "cc1101.h"

CC1101 cc1101;

byte syncWord[2] = {199, 0};

// a flag that a wireless packet has been received
boolean packetAvailable = false;


/* Handle interrupt from CC1101 (INT0) gdo0 on pin2 */
void cc1101signalsInterrupt(void){
  // set the flag that a package is available
  packetAvailable = true;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Intialisation...");
   cc1101.init();
  
 //cc1101.setSyncWord(syncWord, false); //activation de la synchronisation
 //cc1101.setCarrierFreq(CFREQ_433); //changement de frequence
 //cc1101.set_868_GFSK_500_K();       //changement du type de modulation et du debit (modulation GFSK, debit 500 kbauds)
 //cc1101.set_868_GFSK_1_2_K();       //changement du type de modulation et du debit (modulation GFSK, debit 1,2 kbauds)
  cc1101.set_433_GFSK_500_K();        //changement du type de modulation et du debit (modulation GFSK, debit 1,2 kbauds avec frequence 433 Mhz)
  cc1101.setChannel(10);
  cc1101.disableAddressCheck(); //if not specified, will only display "packet received"
  
  attachInterrupt(0, cc1101signalsInterrupt, FALLING);
  Serial.println("RF initialise");
   
}

void loop() {
  // put your main code here, to run repeatedly:
  verificationPaquetRecu();
  delay(100);
}

/**
 * Récupérer le paquet
 */
String receptionPaquet(){
  CCPACKET paquet;
  String data="";
    
    if(cc1101.receiveData(&paquet) > 0){
      if(!paquet.crc_ok) {
        Serial.println("Erreur CRC");
      }
      if(paquet.length > 0){
        if(paquet.length == 60 && paquet.data[55]=='~' && paquet.data[56]=='@' && paquet.data[57]==']' && paquet.data[58]=='#' && paquet.data[59]=='`' ){
          for(int j=0; j<55; j++){
            Serial.write(paquet.data[j]);
            data+=paquet.data[j];
          }
        }
        else{
          for(int j=0; j<paquet.length; j++){
            Serial.write(paquet.data[j]);
            data+=paquet.data[j];
          }
          Serial.println("ici");
        }
      }
    }

    return data;
}

/**
 * Verifier s'il y'a un paquet à recevoir et le récuperer s'il y'en a
 */
void verificationPaquetRecu(){
  if(packetAvailable){
    //Serial.println("Paquet recu");
    
    // Desactivation de la reception
    detachInterrupt(0);
    packetAvailable = false;
    String data = receptionPaquet();

    Serial.println(data);
    //envoyer via ethernet
    //sendEthernetData(data);
    
    // Activation de la reception
    attachInterrupt(0, cc1101signalsInterrupt, FALLING);
  }
}

