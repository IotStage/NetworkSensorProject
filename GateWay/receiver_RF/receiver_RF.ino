#include "EEPROM.h"
#include "cc1101.h"
#include <Wire.h>


//creation d'un objet cc1101
CC1101 cc1101;

byte syncWord[2] = {199, 0};

// a flag that a wireless packet has been received
boolean packetAvailable = false;


/* Handle interrupt from CC1101 (INT0) gdo0 on pin2 */
void cc1101signalsInterrupt(void){
  // set the flag that a package is available
  packetAvailable = true;
}

void setup()
{
  Serial.begin(9600);

  //rejoindre le bus I2C
  Wire.begin();
  
  initialisation();
  
}


void loop()
{
  verificationPaquetRecu();
}


/**
 * Initialiser les modules
 */
void initialisation(){

  Serial.println("Intialisation...");

  
  // initialisation de l'antenne RF
  cc1101.init();

  cc1101.set_433_GFSK_500_K();        //changement du type de modulation et du debit (modulation GFSK, debit 1,2 kbauds avec frequence 433 Mhz)
  cc1101.disableAddressCheck(); //if not specified, will only display "packet received"
  
  attachInterrupt(0, cc1101signalsInterrupt, FALLING);
  Serial.println("RF initialise");

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
          char donnees [56];
          for(int j=0; j<55; j++){
            Serial.write(paquet.data[j]);
            data+=paquet.data[j];
            donnees[j]= (char) paquet.data[j];
            
          }
          donnees[55]='\0';
          sendToEthernet(donnees);
        }
        else{
          char donnees [paquet.length+1];
          for(int j=0; j<paquet.length; j++){
            Serial.write(paquet.data[j]);
            data+=paquet.data[j];
            donnees[j]= (char) paquet.data[j];
          }
          donnees[paquet.length]='\0';
          sendToEthernet(donnees);
          Serial.println("");
          
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

    

    // Activation de la reception
    attachInterrupt(0, cc1101signalsInterrupt, FALLING);
  }
}


void sendToEthernet(char* donnees){
  //char donnees [data.length()];
  //data.toCharArray(donnees, data.length());
  Serial.println("\necriture vers ethernet");
  Wire.beginTransmission(4); // Envoyer vers device #4
  Wire.write(donnees); 
  Wire.endTransmission();    // Arreter la transmission
  delay(2000); // Attendre 2s
}

