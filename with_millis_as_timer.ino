#define BLYNK_PRINT Serial

#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>


#define TRIGGERPIN 5
#define ECHOPIN    4
#define relay 16

int setlvl = 0;
int pmptime = 0;
unsigned long timerOn = 0;


char auth[] = "------------------------";
char ssid[] = "------------------------";
char pass[] = "------------------------";

//reading widget
BLYNK_WRITE(V2)
{
  setlvl = param.asInt();
  Serial.print("setlvl is:");
  Serial.println(setlvl); 
}
BLYNK_WRITE(V3)
{
  pmptime = param.asInt();
  Serial.print("pmptime is:");
  Serial.println(pmptime); 
}

void setup()
{
  // Debug console
  Serial.begin(9600);
  pinMode(relay,OUTPUT);
  digitalWrite(relay,HIGH);
  pinMode(TRIGGERPIN, OUTPUT);
  pinMode(ECHOPIN, INPUT);
  Blynk.begin(auth, ssid, pass);
  }
  
  BLYNK_CONNECTED(){
  Blynk.syncAll();
}

long GetDistance()
{
  //reading sensor
  long duration, distance;
  digitalWrite(TRIGGERPIN, LOW);  
  delayMicroseconds(3); 
  
  digitalWrite(TRIGGERPIN, HIGH);
  delayMicroseconds(12); 
  
  digitalWrite(TRIGGERPIN, LOW);
  duration = pulseIn(ECHOPIN, HIGH);
  distance = (duration/2) / 29.1;
  Serial.print("value :");
  Serial.println(distance);
  Blynk.virtualWrite(V5, distance);
  
  return distance;
}

void loop()
{
  Blynk.run();
  
  delay(3500);
  
  //pump processing 
    if (timerOn > 0)  //still working
  {
    if ((millis() - timerOn > pmptime) || (millis() < timerOn) )  //got to stop
    {    
  Serial.println("PUMP OFF");
      digitalWrite(relay,HIGH); 
        timerOn = 0;
    }   
  }       
  else if (GetDistance() >= setlvl)    //got to check level 
  {
    Serial.println("PUMP ON");
    digitalWrite(relay, LOW);
    timerOn = millis();
  }
  
 }
