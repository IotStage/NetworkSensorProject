
float vout = 0.0;
float vin = 0.0;
float R1 = 10000; //10k
float R2 = 1000; //1000 ohm resistor, I tweaked this
int val_lu = 0;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  Serial.println(getTension());
}
float getTension(){
   val_lu = analogRead(A0);
   vout = (val_lu * 5.0) / 1024.0;
   vin = vout / (R2/(R1+R2)); 
   return vin;
}
