/*
 # Cet code rassemble tous les capteurs
 # Ver   : 2.1.1
 # Product: analog pH meter, turbidity, ORP, EC Meter, Temperature, anemometre, rain jauge
 # Author : Bassirou NGOM
 # email : bassiroungom26@gmail.com
 # Require: Sensor Librairie, DHT and  SX1276 librairie 
 */
 
/************
 * inclusions
 ************/
 #include <SPI.h>
 #include <RH_RF95.h>
 #include <DHT.h>
 #include <sensor.h>
 #include <WSNTemperatureSensor.h>
 #include <String.h>
#include "encrypt.h"

/************
 * Constantes
 ************/
#define SensorPinPH A0
#define SensorPinEC A1
#define SensorPinTurbidity A3
#define SensorPinORP A4
#define SensorPinTemperature 26
#define SensorPinAnemometre 21
#define SensorPinPluviometre 21
#define SensorPinDHT 40
#define SensorPinGirouette A2


#define ADDRESSE_NODE 5
#define ADDRESSE_RELAI 9
#define DEBUG 0


/*************
 * Variables *
 *************/
 Sensor Ph;
 Sensor EC;
 Sensor ORP;
 //Sensor Anemometre;
 Sensor Turbidity;
 Temperature temperature(SensorPinTemperature);
 Sensor girouette;
 DHT dht;
long DELAI = 1000;
int nbBasculement = 0;
int avant = 1;
int value = 0;
int e=0;
String paquet="";
unsigned long temps=0;
unsigned long ATTENTE = 10000;
float humidity = 0.0;
float temp = 0.0;
float temperature2 = 0.0;
long tempsDHT=0;
int i=0;

RH_RF95 rf95;
ENCRYPT encrypt_decrypt;
float frequency = 868.0;
unsigned char encryptkey[16]={0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF,0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF};


/*************
 * Variables ph *
 *************/
 float tableauPh[40];
 int indexPh=0; 
 #define offsetPH 0
unsigned long samplingTime = millis();
unsigned long samplingTime2 = millis();
 

/*************
 * Variables EC *
 *************/
float tableauEC[20];
int indexEC=0; 
unsigned long samplingTimeEC = millis();
unsigned long samplingTimeEC2 = millis();
unsigned long AnalogValueTotal = 0; 
unsigned long AnalogSampleTime=millis(),printTime=millis(),tempSampleTime=millis();
unsigned int AnalogAverage = 0,averageVoltage=0;

/*************
 * Variables ORP *
 *************/
//#define VOLTAGE 5.00    //system voltage
#define offsetORP 0        //zero drift voltage
//#define LED 13         //operating instructions
float orpValue = 0.0;
#define ArrayLenth  100    //times of collection
#define orpPin A1          //orp meter output,connect to Arduino controller ADC pin
float orpArray[ArrayLenth];
int orpArrayIndex=0;

/*****************
 * Variables ORP *
 *****************/
 unsigned long tempsDebut=0;
unsigned long tempsFin=0;
long nbTour=0;
boolean etatPrecedent;
boolean etatActuel;


/*************
 * fonctions *
 *************/
 void updateSensor();
 void sendDataToMaster();
 void getDataFromMaster();
 String getTrame();
 void getBasculement();
 void initRf();
 boolean availablePack();
 void sendPaquet(String paquet, int idClient);
 void updatePh();
 void updateEC();
 void updateORP();
 void updateAnemometre();
 void updateTurbidity();

void setup() {
  Serial.begin(115200);
  initRF();
  // put your setup code here, to run once:
  Ph.init(SensorPinPH);
  EC.init(SensorPinEC);
  ORP.init(SensorPinORP);
  Turbidity.init(SensorPinTurbidity);
  girouette.init(SensorPinGirouette);
  pinMode(SensorPinPluviometre, INPUT);
  pinMode(SensorPinAnemometre, INPUT);
  dht.setup(SensorPinDHT);
  tempsDHT=millis();
  temps=millis();
  attachInterrupt(2, updatePluvio, RISING);
}

void loop() {
  updateSensor();
  Serial.print(ATTENTE);
  Serial.print("=>");
  Serial.print(temps);
  Serial.print("=>");
  Serial.println(millis() - temps);
  
  if(millis() - temps > ATTENTE){
    sendPaquet(getTrame(), ADDRESSE_RELAI);
    Serial.print("paquet ");
    Serial.print(i);
    Serial.println(" sent");
    i++;
    temps = millis();
  }

  if(rf95.waitAvailableTimeout(100))
  {
    Serial.println("Reception d'un message");
    char buf[RH_RF95_MAX_MESSAGE_LEN];//buffer to store the server response message
    uint8_t len = sizeof(buf);// data length
    if (rf95.recv(buf, &len))//check if receive data is correct 
    { 
      Serial.println("decryptage du message "); 
      Serial.println(buf);         
      encrypt_decrypt.btea(buf, -len, encryptkey);
      Serial.print(buf[0]);Serial.print(buf[1]);Serial.print(buf[2]);Serial.println(buf[3]);
      Serial.println(sizeof(buf));
      if(buf[0] == 'C' && buf[1] == 'M' && buf[2] == 'D' && buf[3] == ':' )
      {
        Serial.println(buf);
        char buff2[len-4];
        for(int i = 0; i<len-4; i++)
          buff2[i]=buf[i+4];
        Serial.println(sizeof(buff2));
        String valeur = String(buff2);
        Serial.print("la valeur");
        Serial.println(valeur);
        //ATTENTE=valeur.toInt();
        ATTENTE=(long)valeur.toInt();
        temps=millis();
      }
    }
    
  }
  //delay(DELAI);
}

String getTrame(){
  String trame="";
  trame+=String(Ph.getMesure(), 1);
  trame+=";";
  trame+=String(EC.getMesure(), 1);
  trame+=";";
  trame+=String(ORP.getMesure(), 1);
  trame+=";";
  trame+=String(Turbidity.getMesure(), 1);
  trame+=";";
  trame+=String((temp), 1);
 // float z=analogRead(A2)*(5.0/1024.0);;
  //trame+=String(z, 1);
  trame+=";";
  float y=analogRead(A1)*(5.0/1024.0);;
  trame+=String(y, 1);
  trame+=";";
  trame+=String(girouette.getMesure(), 1);
  trame+=";";
  //trame+=String(humidity, 1);
  trame+=String(analogRead(A3)*(5.0/1024.0), 1);
  trame+=";";
  //trame+=String(temperature2, 1);
  trame+=String(analogRead(A4)*(5.0/1024.0), 1);
  trame+=";";
  float x= analogRead(A0)*(5.0/1024.0);
  trame+=String(x, 1);
  
 return trame; 
}

void updatePluvio(){
  nbBasculement++;
}

void updateSensor(){
  updatePh();
  
  #if(DEBUG > 0)
  Serial.println(F("Ph updated"));
  #endif
  delay(50);
  updateEC();
  #if(DEBUG > 0)
  Serial.println(F("EC updated"));
  #endif
  delay(50);
  updateORP();
  #if(DEBUG > 0)
  Serial.println(F("ORP updated"));
  #endif
  delay(50);
  updateTurbidity();
  #if(DEBUG > 0)
  Serial.println(F("Turbidity updated"));
  #endif
  delay(50);
  updateGirouette();
  #if(DEBUG > 0)
  Serial.println(F("Girouette"));
  #endif
  delay(50);
  updateDHT();
  #if(DEBUG > 0)
  Serial.println(F("DHT updated"));
  #endif
  delay(50);
  updateAnemometre();
  #if(DEBUG > 0)
  Serial.println(F("Anemometre updated"));
  #endif
  delay(50);
}

void initRF(){
  Serial.println(F("Debut initialisation RF"));
  if (!rf95.init())
    Serial.println("init failed");
   // Setup ISM frequency
  rf95.setFrequency(frequency);
  // Setup Power,dBm
  rf95.setTxPower(13);
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  Serial.print("Configuration terminee");
}

void sendPaquet(String paquet, int idClient){
  int len = paquet.length()+1;
  char buff[len];
  Serial.println(paquet);
  paquet.toCharArray(buff, len);
  char buff2[len+4]; 
  buff2[0] = 'I';
  buff2[1] = 'N';
  buff2[2] = 'F';
  buff2[3] = ':';
  for(int i=4; i<len+4; i++){
    buff2[i] = buff[i-4];
  }
  encrypt_decrypt.btea(buff2, len+4, encryptkey);
  rf95.send(buff2, len+4);
  Serial.println(buff2);
  //e = sx1276.sendPacketTimeout(idClient, buff);
  #if (DEBUG > 0)
    Serial.print("Packet sent, state ");
    //Serial.println(e, DEC);
    Serial.println(buff);
  #endif
  
}


void updateDHT(){
  if(millis() - tempsDHT > dht.getMinimumSamplingPeriod() ){
    humidity =random(70, 80);
    temperature2 = random(25, 40);
    tempsDHT = millis();
  }
}

float getDirectionVent(){
  float valeur = analogRead(girouette.pinSensor)*(5.0/1023.0);
  if(valeur<=0.32)
    return 112.5;
  else if(valeur <= 0.41)
    return 67.5;
  else if(valeur <= 0.45)
    return 90.0;
  else if(valeur <= 0.62)
    return 157.5; 
  else if(valeur <= 0.90)
    return 135.0;
  else if(valeur <= 1.19)
    return 202.5;
  else if(valeur <= 1.40)
    return 180.0;
  else if(valeur <= 1.98)
    return 22.5;
  else if(valeur <= 2.25)
    return 45.0;
  else if(valeur <= 2.93)
    return 147.5;
  else if(valeur <= 3.08)
    return 225.0;
  else if(valeur <= 3.43)
    return 137.5;
  else if(valeur <= 3.84)
    return 0.0;
  else if(valeur <= 4.04)
    return 292.5;
  else if(valeur <= 4.62)
    return 270.0;
  else if(valeur <= 4.78)
    return 315.0;
  
}

void updateAnemometre(){
  etatActuel = digitalRead(SensorPinAnemometre);
  if(etatActuel==1 ){
    tempsDebut=millis();
    while(millis()-tempsDebut < 1000){
      int etat = digitalRead(SensorPinAnemometre);
      //digitalWrite(13, etat);
      nbTour = (etatActuel !=  etat) ? nbTour+1 : nbTour; 
      etatActuel = etat; 
      #if(DEBUG > 0)  
        Serial.print(etat);
        Serial.print(" , ");
        Serial.println(nbTour);
      #endif
    } 
    #if(DEBUG > 0)  
        Serial.println(F("end updated"));
      #endif
  }
}

float getAnemometreMesure(){
  float val=2*3.14*7.2*0.01*nbTour/4;
  nbTour=0;
  return val;
}

void updateGirouette(){
  girouette.sensorValue = getDirectionVent();
}

void updateTurbidity(){
  int sensorValue = analogRead(Turbidity.pinSensor);
  float voltage = sensorValue * (5000.0 / 1023.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  Turbidity.sensorValue = voltage*0.1035-0.292;
}

void updatePh()
{
  static float voltage;

  if(millis()-samplingTime > 20)
  {
    tableauPh[indexPh++]=analogRead(Ph.pinSensor);
    if(indexPh==40)
        indexPh=0;
    samplingTime=millis();
  }
  if(millis()-samplingTime2 > 800){
    
    voltage = Ph.moyenne(tableauPh, 40)*5.0/1024;
    Ph.sensorValue = 3.5*voltage+offsetPH;
    samplingTime2=millis();
  }

  Ph.sensorValue=random(5, 7);
}

void updateEC(){
  
  /*
   Every once in a while,sample the analog value and calculate the average.
  */
  if(millis()-samplingTimeEC>= 25 )  
  {
    samplingTimeEC=millis();
     // subtract the last reading:
    AnalogValueTotal = AnalogValueTotal - tableauEC[indexEC];
    tableauEC[indexEC] = analogRead(EC.pinSensor);
    AnalogValueTotal = AnalogValueTotal + tableauEC[indexEC];
    indexEC = indexEC + 1;
    // if we're at the end of the array...
    if (indexEC >= 20)
      indexEC = 0;
    AnalogAverage = AnalogValueTotal / 20;
  }
  /*
   Every once in a while,MCU read the temperature from the DS18B20 and then let the DS18B20 start the convert.
   Attention:The interval between start the convert and read the temperature should be greater than 750 millisecond,or the temperature is not accurate!
  */
   if(millis()-tempSampleTime>=850) 
  {
    tempSampleTime=millis();
    temp = temperature.getTemperature();  // read the current temperature from the  DS18B20
    temperature.updateTemperature(0);                   //after the reading,start the convert for next reading
  }
    
   /*
   Every once in a while,print the information on the serial monitor.
   */
  if(millis()-printTime>=700)
  {
    printTime=millis();
    averageVoltage=AnalogAverage*(float)5000/1024;
    #if(DEBUG > 0)
    Serial.print("Analog value:");
    Serial.print(AnalogAverage);   //analog average,from 0 to 1023
    Serial.print("    Voltage:");
    Serial.print(averageVoltage);  //millivolt average,from 0mv to 4995mV
    Serial.print("mV    ");
    Serial.print("temp:");
    Serial.print(temp);    //current temperature
    Serial.print("^C     EC:");
    #endif
  float TempCoefficient=1.0+0.0185*(temp-25.0);    //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.0185*(fTP-25.0));
  float CoefficientVolatge=(float)averageVoltage/TempCoefficient;   
  if(CoefficientVolatge<150);//Serial.println("No solution!");   //25^C 1413us/cm<-->about 216mv  if the voltage(compensate)<150,that is <1ms/cm,out of the range
  else if(CoefficientVolatge>3300);//Serial.println("Out of the range!");  //>20ms/cm,out of the range
  else
  { 
    if(CoefficientVolatge<=448)EC.sensorValue=6.84*CoefficientVolatge-64.32;   //1ms/cm<EC<=3ms/cm
    else if(CoefficientVolatge<=1457)EC.sensorValue=6.98*CoefficientVolatge-127;  //3ms/cm<EC<=10ms/cm
    else EC.sensorValue=5.3*CoefficientVolatge+2278;                           //10ms/cm<EC<20ms/cm
    EC.sensorValue/=1000;    //convert us/cm to ms/cm
    #if(DEBUG > 0)
    Serial.print(EC.sensorValue,2);  //two decimal
    Serial.println("ms/cm");
    #endif
  }
  }
   EC.sensorValue=random(10, 14);
}


void updateORP(){
  static unsigned long orpTimer=millis();   //analog sampling interval
  static unsigned long printTime=millis();
  if(millis() >= orpTimer)
  {
    orpTimer=millis()+20;
    orpArray[orpArrayIndex++]=analogRead(orpPin);    //read an analog value every 20ms
    if (orpArrayIndex==ArrayLenth) {
      orpArrayIndex=0;
    }   
    orpValue=((30*(double)5.0*1000)-(75*ORP.moyenne(orpArray, ArrayLenth)*5.0*1000/1024))/75-offsetORP;  

    //convert the analog value to orp according the circuit
  }
  if(millis() >= printTime)   //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
  {
  printTime=millis()+800;
  ORP.sensorValue=orpValue;
  ORP.sensorValue=random(1, 100);
  
  #if(DEBUG > 0)
    Serial.print("ORP: ");
    Serial.println((int)orpValue);
        Serial.println("mV");
       // digitalWrite(LED,1-digitalRead(LED));
  #endif
  }
}


