
/*
 # Cet code rassemble tous les capteurs
 # Ver   : 2.0
 # Product: analog pH meter, turbidity, ORP, EC Meter, Temperature, anemometre, rain jauge
 # Author : Bassirou NGOM
 # email : bassiroungom26@gmail.com
 # Require: WSN Librairie
 */
/************
 * inclusions
 ************/
 #include <WSNPHSensor.h>
 #include <WSNORPSensor.h>
 #include <WSNECSensor.h>
 #include <WSNTemperatureSensor.h>
 #include <WSNAnemometreSensor.h>
 #include <WSNTurbiditySensor.h>
 #include <EEPROM.h>
 #include <cc1101.h> 

/************
 * Constantes
 ************/
#define SensorPinPH A0
#define SensorPinEC A1
#define SensorPinTurbidity A3
#define SensorPinORP A4
#define SensorPinTemperature 5
#define SensorPinAnemometre 3
#define SensorPinPluviometre 7
#define WIREADD 8


/*************
 * Variables *
 *************/
Ph ph(SensorPinPH);
EC ec(SensorPinEC, SensorPinTemperature);
ORP orp(SensorPinORP);
Anemometre anemometre(SensorPinAnemometre);
Turbidity turbidity(SensorPinTurbidity);
int DELAI = 1000;
int nbBasculement = 0;
int avant = 1;
int value = 0;
CC1101 cc1101;
byte syncWord[2] = {199, 0};
boolean packetAvailable = false;
CCPACKET paquet;


/*************
 * fonctions *
 *************/
 void updateSensor();
 void sendDataToMaster();
 void getDataFromMaster();
 String getTrame();
 void getBasculement();
 void initRf();
 void cc1101signalsInterrupt(void);
 void envoiPaquet();
 void formatPaquet(String message);
 
/****************************
 * definition des fonctions *
 ****************************/
void setup(){
  Serial.begin(9600);
  initRF();
  anemometre.init();
  ec.init();
  pinMode(SensorPinPluviometre, INPUT);
  
}

void loop(){
  String trame= getTrame();
  updateSensor();
  //delay(DELAI);
  formatPaquet(trame);
  Serial.println("paquet sent");
  delay(DELAI);
  
}

void initRF(){
  cc1101.init();
  cc1101.set_433_GFSK_500_K();        //changement du type de modulation et du debit (modulation GFSK, debit 1,2 kbauds avec frequence 433 Mhz)
  cc1101.setChannel(10);
  cc1101.disableAddressCheck(); //if not specified, will only display "packet received"
  attachInterrupt(0, cc1101signalsInterrupt, FALLING);
  Serial.println("Initialisarion antenne RF terminee...");
}


/* Handle interrupt from CC1101 (INT0) gdo0 on pin2 */
void cc1101signalsInterrupt(void){
  // set the flag that a package is available
  packetAvailable = true;
}


void envoiPaquet() {
  if(cc1101.sendData(paquet)){
    Serial.println("Envoi termine");
   }else{
    Serial.println("Echec de l'envoi");
  }
}

/***
 * format segmentatoin des donnees a envoyer en paquet, 
 * ajout d'une entete specifique (~@]#) pour limiter les paquets
 */
void formatPaquet(String message){
  Serial.print("message :");
  Serial.println(message);
  if(message.length()<61){
    Serial.print("Taille ");
    Serial.println(message.length());
    paquet.length=message.length();
    message.getBytes(paquet.data, message.length()+1);
    envoiPaquet();
  }
  else{
    String partie1 = message.substring(0, 55);
    String partie2 = message.substring(55);
    partie1+="~@]#`";
    formatPaquet(partie1);
    //delay(500);
    formatPaquet(partie2);
  }
}


void updateSensor(){
  ph.updatePh();
  ec.updateEC();
  orp.updateORP();
  anemometre.updateAnemometre();
  turbidity.updateTurbidity();
  getBasculement();
}

String getTrame(){
  String trame="";
  trame+="phv="+String(ph.getMesure(),2);
  trame+=";";
  trame+="orp="+String(orp.getMesure(),2);
  trame+=";";
  trame+="ecv="+String(ec.getMesure(),2);
  trame+=";";
  trame+="tem="+String(ec.getTemperature(), 2);
  trame+=";";
  trame+="vit="+String(anemometre.getMesure(),2);
  trame+=";";
  trame+="tur="+String(turbidity.getMesure(),2);
  trame+=";";
  trame+="plu="+String(nbBasculement);
  nbBasculement=0;
  return trame;
  
}

void getBasculement(){
  value = digitalRead(SensorPinPluviometre);
  if (value == 0 && avant == 1) 
    nbBasculement++;
  avant = value;
}

