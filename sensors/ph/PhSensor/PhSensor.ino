/*
 # Cette code permet de recuper la valeur du ph et le votage correspondant 
 # Ver   : 1.0
 # Product: analog pH meter
 # SKU   : SEN0169
 # Author : Bassirou NGOM 
*/

#define SensorPin A0  //le pin ou est branche le capteur
#define Offset 2.50  //compensation de la deviation
#define LED 13      //le pin du led sur la carte
#define intervalMesure 20 //intervalle de ou on lit les valeurs sur le pin SensorPin
#define intervalConvertion 800 //intervalle ou les valeurs prises seront convertis en ph
#define tailleEchantillon  40  //taille des echantillons

int tableauPh[tailleEchantillon];  //tableau stokage des echantillons
int indexPh=0;  

void setup(void)
{
 pinMode(LED,OUTPUT);  
 Serial.begin(9600);  
 Serial.println("pH meter ");  //Test the serial monitor

}

void loop(void)
{
 static unsigned long samplingTime = millis();
 static unsigned long printTime = millis();
 static float pHValue,voltage;
 
 if(millis()-samplingTime > intervalMesure){
   tableauPh[indexPh++]=analogRead(SensorPin);
   if(indexPh==tailleEchantillon)
    indexPh=0;
   voltage = moyenne(tableauPh, tailleEchantillon)*5.0/1024;
   pHValue = 3.5*voltage+Offset;
   samplingTime=millis();
 }

 if(millis() - printTime > intervalConvertion)  //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
 {
 Serial.print("Voltage:");
 Serial.print(voltage,2);
 Serial.print("  pH value: ");
 Serial.println(pHValue,2);
 digitalWrite(LED,digitalRead(LED)^1);
 printTime=millis();
 }

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
