#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 4

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

const int purificator = 13;
const int buzzerPin = 3;
//waterflowsensor variables
byte waterflowInterrupt = 0;  // 0 = digital pin 2
byte waterflowPin       = 2;
float calibrationFactor = 4.5;
volatile byte pulseCount;  
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
unsigned long oldTime;

float oldTimePurif = 0;
float drinkTime = 0;

void setup() {
  Serial.begin(9600);

  // digitalWrite(statusLed, HIGH);
  
  pinMode(waterflowPin, INPUT);
  pinMode(purificator, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  pinMode(3, OUTPUT);
  digitalWrite(waterflowPin, HIGH);

  pulseCount        = 0;
  flowRate          = 0.0;
  flowMilliLitres   = 0;
  totalMilliLitres  = 0;
  oldTime           = 0;

  tone(buzzerPin, 1000, 2000);
  attachInterrupt(waterflowInterrupt, pulseCounter, FALLING);
}

void loop() {
  digitalWrite(3, HIGH);
  //waterflow sensor
   if((millis() - oldTime) > 1000)
  { 
    detachInterrupt(waterflowInterrupt);
    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
    oldTime = millis();
    flowMilliLitres = (flowRate / 60) * 1000;
    totalMilliLitres += flowMilliLitres;
      
    unsigned int frac;
    Serial.print("Flow rate: ");
      // Print the integer part of the variabl
    Serial.print("\t"); 		  // Print tab space

    Serial.print("Output Liquid Quantity: ");        
    
    Serial.print("\t"); 		  // Print tab space
	  Serial.print(totalMilliLitres/1000);
	  Serial.print("L");
    Serial.print("    ||    ");
    

    pulseCount = 0;
    
    attachInterrupt(waterflowInterrupt, pulseCounter, FALLING);
  }


  //temperature
  // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
  sensors.requestTemperatures(); 
  
  Serial.print("Celsius temperature: ");
  // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire
  Serial.println(" - Fahrenheit temperature: ");
  Serial.print(int(flowRate));
  Serial.print("L/min");
  Serial.print("    ||    ");
  Serial.print(totalMilliLitres);
  Serial.print("mL"); 
  Serial.print("                            ||            ");
  Serial.print(sensors.getTempCByIndex(0)); 
  Serial.print("           ||            ");
  Serial.println(sensors.getTempFByIndex(0));
  delay(1000);

  if(millis() - oldTimePurif > 360000){
    digitalWrite(purificator, HIGH);
    delay(20000);
    digitalWrite(purificator, LOW);
    oldTimePurif = millis();
  }

  if(millis()- drinkTime > 3600000){
    tone(buzzerPin, 440); // A4
    delay(1000);

    tone(buzzerPin, 587); // D4
    delay(1000);

    tone(buzzerPin, 440); // A4
    delay(1000);

    tone(buzzerPin, 587); // D4
    delay(1000);
    noTone(buzzerPin);
    drinkTime = millis();
  }
}


void pulseCounter()
{
  pulseCount++;
}