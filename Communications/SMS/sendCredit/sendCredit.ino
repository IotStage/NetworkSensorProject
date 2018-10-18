#include <string.h>
#include <GSM.h>
#include <SoftwareSerial.h>

#define NUMBERSERVER "774686888"
#define PINNUMBER ""
GSM gsmAccess;
GSM_SMS sms;
SoftwareSerial cell(2,3);
String reponse="";
char incoming_char = 0;
String ligne="";
String serial="";
long temps=0;

void initGSM(){
   Serial.println("Initialisation carte GSM");

  // connection state
  boolean notConnected = true;

  // Start GSM shield
  // If your SIM has PIN, pass it as a parameter of begin() in quotes
  while (notConnected) {
    if (gsmAccess.begin(PINNUMBER) == GSM_READY) {
      notConnected = false;
    } else {
      Serial.println("Not connected");
      delay(1000);
    }
  }

  Serial.println("GSM initialized");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  initGSM();
   // Reset
  Serial.println("Start Reset");
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);
  delay(12000);
  digitalWrite(7, LOW);
  delay(1000);
  Serial.println("End Reset"); 

  cell.begin(9600);
  Serial.println("Enter your AT commands (with CR & NL)...");
  temps = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(cell.available() > 0){
    incoming_char = cell.read();
    if((incoming_char >= ' ') && (incoming_char<='z'))
      reponse+=incoming_char;
    else{
      
       if(incoming_char == 10){
        reponse+="\n";
       }
        
    }
    //}
    //Serial.println(reponse);
   }
if(Serial.available() > 0)
  {
    serial = Serial.readString();
    Serial.println(serial);
    //if(serial=="credit")
      executeUSSD();
  }
if(millis() - temps > 10000){
  //Serial.print(i);
  Serial.print("la valeur ");
  //Serial.println(reponse);
  afficherCredit();
  temps = millis();
  }
   
}

void sendSMS(String donnees){
    char buff[donnees.length()+1];
    donnees.toCharArray(buff,donnees.length()+1);
    sms.beginSMS(NUMBERSERVER);
    sms.print(buff);
    sms.endSMS();
    Serial.println(buff);
  
  
}


void executeUSSD(){
  cell.print("AT+CUSD=1,\"#123#\"\r\n");
  delay(500);
}

void traitement(){
ligne="";
  char buff[reponse.length()+1];
  reponse.toCharArray(buff, reponse.length()+1);
  int c=0;
  for(int i=0; i<=reponse.length(); i++){
    if(buff[i] =='\n') c++;
    if(c==1)
      ligne+=buff[i+1];
  }
  Serial.println(ligne);
  //Serial.println();
}

void afficherCredit(){
  traitement();
   String montant="";
  String date="";
  char buff[ligne.length()+1];
  ligne.toCharArray(buff, ligne.length()+1);
  int c=0;
  int d=0;
  int arr=0;
   for(int i=0; i<=ligne.length(); i++){
    if(buff[i] == ':')c++;
    else if(buff[i] == 'e' && buff[i-1] == 'l'  )d++;
    if(c==2){
      if(buff[i]!='F' && arr==0) montant+=buff[i];
      else arr=1; 
    }
    if(d==1){
      if(buff[i]!='.') date+=buff[i];
      else break;
    }
   }
   Serial.println(montant.substring(2));
   Serial.println(date.substring(2));
}

