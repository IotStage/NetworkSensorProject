#include <SoftwareSerial.h>
#include <string.h>

char incoming_char = 0;
SoftwareSerial cell(2,3);

void setup()
{
  // Initialize serial ports for communication.
  Serial.begin(9600);

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
}

void loop()
{
  if(cell.available() > 0)
  {
    incoming_char = cell.read();
    if((incoming_char >= ' ') && (incoming_char<='z'))
      Serial.print(incoming_char);
    else
    {
      Serial.print("%");
      Serial.print((int) incoming_char);
      Serial.print("%");
      if(incoming_char == 10)
        Serial.println();
    }
  }

  if(Serial.available() > 0)
  {
    incoming_char = Serial.read();
    cell.print(incoming_char);
  }
}
