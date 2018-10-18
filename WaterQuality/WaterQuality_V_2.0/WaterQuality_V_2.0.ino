
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
 #include <Wire.h>
 

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
uint8_t DELAI = 1000;
int nbBasculement = 0;
int avant = 1;
int value = 0;
char mode=' ';


/*************
 * fonctions *
 *************/
 void updateSensor();
 void sendDataToMaster();
 void getDataFromMaster();
 String getTrame();
 void getBasculement();

 
/****************************
 * definition des fonctions *
 ****************************/
void setup(){
  Serial.begin(9600);
  Wire.begin(WIREADD);
  Wire.onRequest(sendDataToMaster);
  Wire.onReceive(getDataFromMaster);
  anemometre.init();
  ec.init();
  pinMode(SensorPinPluviometre, INPUT);
  
}

void loop(){
  updateSensor();
  delay(DELAI);
  Serial.println(getTrame());
}

void updateSensor(){
  ph.updatePh();
  //ec.updateEC();
  orp.updateORP();
  /*anemometre.updateAnemometre();
  turbidity.updateTurbidity();
  getBasculement();*/
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

void sendDataToMaster(){
   /*switch(mode){
    case '1': Wire.write("mode 1 : send temp");break; //18 bytes
    case '2': Wire.write("mode 2 : send ORP ");break;
    case '3': Wire.write("mode 3 : send EC  ");break;
    case '4': Wire.write("mode 4 : send PH  ");break;
    default: Wire.write("-> temp,ORP,EC,PH ");
   }*/
  String msg = getTrame();
   int len = msg.length()+1;
   byte buff[len];
   msg.toCharArray(buff, len);
   Wire.write(buff, len);
   Wire.flush();
   
}

void getDataFromMaster(){
  
}


void getBasculement(){
  value = digitalRead(SensorPinPluviometre);
  if (value == 0 && avant == 1) 
    nbBasculement++;
  avant = value;
}

