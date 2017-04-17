/*
 # CEt code rassemble tous les capteurs
 # Ver   : 1.0
 # Product: analog pH meter, turbidity, ORP, EC Meter, Temperature
 # Author : Bassirou NGOM
 # mail : bassiroungom26@gmail.com
 */
 //inclusion
#include <OneWire.h>

#define SensorPinTurbidity A2
//#define SensorPinEC A1
#define SensorPinORP A3
#define SensorPinPH A0  //le pin ou est branche le capteur PH
const byte BROCHE_ONEWIRE = 2; 

#define OffsetPH 0.00  //compensation de la deviation
#define LED 13      //le pin du led sur la carte
#define intervalMesure 20 //intervalle de ou on lit les valeurs sur le pin SensorPinPH
#define intervalConvertion 800 //intervalle ou les valeurs prises seront convertis en ph
#define tailleEchantillon  40  //taille des echantillons
int tableauPh[tailleEchantillon];  //tableau stokage des echantillons
int indexPh=0; 

//EC
#define StartConvert 0
#define ReadTemperature 1

const byte numReadings = 20;     //the number of sample times
byte SensorPinEC = A1;  //EC Meter analog output,pin on analog 1
byte DS18B20_Pin = 7; //DS18B20 signal, pin on digital 7
unsigned int AnalogSampleInterval=25,printInterval=700,tempSampleInterval=850;  //analog sample interval;serial print interval;temperature sample interval
unsigned int readings[numReadings];      // the readings from the analog input
byte index = 0;                  // the index of the current reading
unsigned long AnalogValueTotal = 0;                  // the running total
unsigned int AnalogAverage = 0,averageVoltage=0;                // the average
unsigned long AnalogSampleTime,printTime,tempSampleTime;
float temperature,ECcurrent; 

OneWire dsEC(DS18B20_Pin);  // on digital pin 2

/* Broche du bus 1-Wire */
 // la temperature

/* Code de retour de la fonction getTemperature() */
enum DS18B20_RCODES {
  READ_OK,  // Lecture ok
  NO_SENSOR_FOUND,  // Pas de capteur
  INVALID_ADDRESS,  // Adresse reçue invalide
  INVALID_SENSOR  // Capteur invalide (pas un DS18B20)
};


/* Création de l'objet OneWire pour manipuler le bus 1-Wire */
OneWire ds(BROCHE_ONEWIRE);


/**
 * Fonction de lecture de la température via un capteur DS18B20.
 */
byte getTemperature(float *temperature, byte reset_search) {
  byte data[9], addr[8];
  // data[] : Données lues depuis le scratchpad
  // addr[] : Adresse du module 1-Wire détecté
  
  /* Reset le bus 1-Wire ci nécessaire (requis pour la lecture du premier capteur) */
  if (reset_search) {
    ds.reset_search();
  }
 
  /* Recherche le prochain capteur 1-Wire disponible */
  if (!ds.search(addr)) {
    // Pas de capteur
    return NO_SENSOR_FOUND;
  }
  
  /* Vérifie que l'adresse a été correctement reçue */
  if (OneWire::crc8(addr, 7) != addr[7]) {
    // Adresse invalide
    return INVALID_ADDRESS;
  }
 
  /* Vérifie qu'il s'agit bien d'un DS18B20 */
  if (addr[0] != 0x28) {
    // Mauvais type de capteur
    return INVALID_SENSOR;
  }
 
  /* Reset le bus 1-Wire et sélectionne le capteur */
  ds.reset();
  ds.select(addr);
  
  /* Lance une prise de mesure de température et attend la fin de la mesure */
  ds.write(0x44, 1);
  delay(800);
  
  /* Reset le bus 1-Wire, sélectionne le capteur et envoie une demande de lecture du scratchpad */
  ds.reset();
  ds.select(addr);
  ds.write(0xBE);
 
 /* Lecture du scratchpad */
  for (byte i = 0; i < 9; i++) {
    data[i] = ds.read();
  }
   
  /* Calcul de la température en degré Celsius */
  *temperature = ((data[1] << 8) | data[0]) * 0.0625; 
  
  // Pas d'erreur
  return READ_OK;
}

 float getPh(){
   static unsigned long samplingTime = millis();
   static unsigned long printTime = millis();
   static float pHValue,voltage;
   
   if(millis()-samplingTime > intervalMesure){
     tableauPh[indexPh++]=analogRead(SensorPinPH);
     if(indexPh==tailleEchantillon)
      indexPh=0;
     voltage = moyenne(tableauPh, tailleEchantillon)*5.0/1024;
     pHValue = 3.5*voltage+OffsetPH;
     samplingTime=millis();
   }
  
   if(millis() - printTime > intervalConvertion)  //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
   {
   //Serial.print("VoltagePH:");
   //Serial.print(voltage,2);
   Serial.print(" pH: ");
   Serial.print(pHValue,2);
   digitalWrite(LED,digitalRead(LED)^1);
   printTime=millis();
   }

   return pHValue;
 }
 double moyenne(int* arr, int number){
 int i;
 int max,min;
 double avg;
 long amount=0;
 if(number<=0){
  Serial.println("Error number for the array to avraging!/n");
  return 0;
}

 if(number<5){  //less than 5, calculated directly statistics
  for(i=0;i<number;i++){
    amount+=arr[i];
  }
  avg = amount/number;
  return avg;
 }else{
  
   if(arr[0]<arr[1]){
    min = arr[0];max=arr[1];
   }
   else{
     min=arr[1];max=arr[0];
    }
    
 for(i=2;i<number;i++){
  if(arr[i]<min){
    amount+=min;  //arr<min
    min=arr[i];
  }else {
    if(arr[i]>max){
      amount+=max;  //arr>max
      max=arr[i];
    }else{
      amount+=arr[i]; //min<=arr<=max
    }
  }//if

 }//for

 avg = (double)amount/(number-2);

 }//if

 return avg;

}


/**
 * Cette fonction retourne la valeur du capteur de turbidite 
 */

float getTurbidity(){
  int sensorValue = analogRead(SensorPinTurbidity);// read the input on analog pin 0:
  float voltage = sensorValue * (5.0 / 1024.0);
  return voltage;//150.0*voltage - 80;//(NTU)
}


#define VOLTAGE 5.00    //system voltage
#define OFFSET 7        //zero drift voltage
//#define LED 13         //operating instructions

double orpValue;

#define ArrayLenth  40    //times of collection
#define orpPin 1          //orp meter output,connect to Arduino controller ADC pin

int orpArray[ArrayLenth];
int orpArrayIndex=0;

double avergearray(int* arr, int number){
  int i;
  int max,min;
  double avg;
  long amount=0;
  if(number<=0){
    printf("Error number for the array to avraging!/n");
    return 0;
  }
  if(number<5){   //less than 5, calculated directly statistics
    for(i=0;i<number;i++){
      amount+=arr[i];
    }
    avg = amount/number;
    return avg;
  }else{
    if(arr[0]<arr[1]){
      min = arr[0];max=arr[1];
    }
    else{
      min=arr[1];max=arr[0];
    }
    for(i=2;i<number;i++){
      if(arr[i]<min){
        amount+=min;        //arr<min
        min=arr[i];
      }else {
        if(arr[i]>max){
          amount+=max;    //arr>max
          max=arr[i];
        }else{
          amount+=arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount/(number-2);
  }//if
  return avg;
}


int getORP(){
  static unsigned long orpTimer=millis();   //analog sampling interval
  static unsigned long printTime=millis();
  if(millis() >= orpTimer)
  {
    orpTimer=millis()+20;
    orpArray[orpArrayIndex++]=analogRead(SensorPinORP);    //read an analog value every 20ms
    if (orpArrayIndex==ArrayLenth) {
      orpArrayIndex=0;
    }   
    orpValue=((30*(double)VOLTAGE*1000)-(75*avergearray(orpArray, ArrayLenth)*VOLTAGE*1000/1024))/75-OFFSET;  

    //convert the analog value to orp according the circuit
  }
  if(millis() >= printTime)   //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
  {
  printTime=millis()+800;
  //Serial.print("ORP: ");
  //Serial.print((int)orpValue);
   //     Serial.println("mV");
        digitalWrite(LED,1-digitalRead(LED));
  }

  return (int)orpValue;
}

void initEC(){
   for (byte thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0;
  TempProcess(StartConvert);   //let the DS18B20 start the convert
  AnalogSampleTime=millis();
  printTime=millis();
  tempSampleTime=millis();
}


/*
ch=0,let the DS18B20 start the convert;ch=1,MCU read the current temperature from the DS18B20.
*/
float TempProcess(bool ch)
{
  //returns the temperature from one DS18B20 in DEG Celsius
  static byte data[12];
  static byte addr[8];
  static float TemperatureSum;
  if(!ch){
          if ( !dsEC.search(addr)) {
              Serial.println("no more sensors on chain, reset search!");
              dsEC.reset_search();
              return 0;
          }      
          if ( OneWire::crc8( addr, 7) != addr[7]) {
              Serial.println("CRC is not valid!");
              return 0;
          }        
          if ( addr[0] != 0x10 && addr[0] != 0x28) {
              Serial.print("Device is not recognized!");
              return 0;
          }      
          dsEC.reset();
          dsEC.select(addr);
          dsEC.write(0x44,1); // start conversion, with parasite power on at the end
  }
  else{  
          byte present = dsEC.reset();
          dsEC.select(addr);    
          dsEC.write(0xBE); // Read Scratchpad            
          for (int i = 0; i < 9; i++) { // we need 9 bytes
            data[i] = dsEC.read();
          }         
          dsEC.reset_search();           
          byte MSB = data[1];
          byte LSB = data[0];        
          float tempRead = ((MSB << 8) | LSB); //using two's compliment
          TemperatureSum = tempRead / 16;
    }
          return TemperatureSum;  
}

float getEC(){
  float ec = 0.0;
  /*
   Every once in a while,sample the analog value and calculate the average.
  */
  if(millis()-AnalogSampleTime>=AnalogSampleInterval)  
  {
    AnalogSampleTime=millis();
     // subtract the last reading:
    AnalogValueTotal = AnalogValueTotal - readings[index];
    // read from the sensor:
    readings[index] = analogRead(SensorPinEC);
    // add the reading to the total:
    AnalogValueTotal = AnalogValueTotal + readings[index];
    // advance to the next position in the array:
    index = index + 1;
    // if we're at the end of the array...
    if (index >= numReadings)
    // ...wrap around to the beginning:
    index = 0;
    // calculate the average:
    AnalogAverage = AnalogValueTotal / numReadings;
  }
  /*
   Every once in a while,MCU read the temperature from the DS18B20 and then let the DS18B20 start the convert.
   Attention:The interval between start the convert and read the temperature should be greater than 750 millisecond,or the temperature is not accurate!
  */
   if(millis()-tempSampleTime>=tempSampleInterval) 
  {
    tempSampleTime=millis();
    temperature = TempProcess(ReadTemperature);  // read the current temperature from the  DS18B20
    TempProcess(StartConvert);                   //after the reading,start the convert for next reading
  }
   /*
   Every once in a while,print the information on the serial monitor.
  */
  if(millis()-printTime>=printInterval)
  {
    printTime=millis();
    averageVoltage=AnalogAverage*(float)5000/1024;
    //Serial.print("Analog value:");
    //Serial.print(AnalogAverage);   //analog average,from 0 to 1023
    //Serial.print("    Voltage:");
    //Serial.print(averageVoltage);  //millivolt average,from 0mv to 4995mV
    //Serial.print("mV    ");
    Serial.print(" temperature:");
    Serial.print(temperature);    //current temperature
    Serial.print(" C     EC:");
    
    float TempCoefficient=1.0+0.0185*(temperature-25.0);    //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.0185*(fTP-25.0));
    float CoefficientVolatge=(float)averageVoltage/TempCoefficient;   
    if(CoefficientVolatge<150)Serial.println("No solution! ");   //25^C 1413us/cm<-->about 216mv  if the voltage(compensate)<150,that is <1ms/cm,out of the range
    else if(CoefficientVolatge>3300)Serial.println("Out of the range! ");  //>20ms/cm,out of the range
    else
    { 
      if(CoefficientVolatge<=448)ECcurrent=6.84*CoefficientVolatge-64.32;   //1ms/cm<EC<=3ms/cm
      else if(CoefficientVolatge<=1457)ECcurrent=6.98*CoefficientVolatge-127;  //3ms/cm<EC<=10ms/cm
      else ECcurrent=5.3*CoefficientVolatge+2278;                           //10ms/cm<EC<20ms/cm
      ECcurrent/=1000;    //convert us/cm to ms/cm
      ec = ECcurrent;
      Serial.print(ECcurrent,2);  //two decimal
      Serial.println(" ms/cm ");
    }
  }
  return ec;
}

void setup() {
  // put your setup code here, to run once:
 pinMode(LED,OUTPUT);  
 Serial.begin(9600); 
 initEC();
}

void loop() {

  Serial.print("Mesure  : ORP =");
  Serial.print(getORP());
  Serial.print("mV Turbidite = ");
  Serial.print(getTurbidity());
  Serial.print("V ");
  getPh();
  getEC();
  delay(1000);
   /*float temperature;
  /* Lit la température ambiante à ~1Hz 
  if (getTemperature(&temperature, true) != READ_OK) {
    Serial.println(F("Erreur de lecture du capteur"));
    return;
  }
*/
  /* Affiche la température 
  Serial.print(F("Temperature : "));
  Serial.print(temperature, 2);
  Serial.write('*'); // Caractère degré
  Serial.write('C');
  Serial.println(", ph :  ");
  Serial.println(getPh());
*/
  
}
