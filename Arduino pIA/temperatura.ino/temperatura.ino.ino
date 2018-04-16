#include <OneWire.h>
#include <DallasTemperature.h>


#define ONE_WIRE_BUS 7

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

int relay_pin = 4;

void setup() {
  pinMode(relay_pin, OUTPUT);          // sets the digital pin 5 as output
  Serial.begin(9600);
  sensors.begin();
}

float checktemp() {
  sensors.requestTemperatures();
  return(sensors.getTempCByIndex(0));
}


float temperatura;
float temperatura_anterior = checktemp();

void loop() {
  temperatura = checktemp();
  Serial.print("Temperature is: ");
  Serial.println(temperatura); // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire
  Serial.print("DeltaT is: ");
  Serial.println(temperatura-temperatura_anterior); // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire
  temperatura_anterior = temperatura;
  //Update value every 1 sec.
  digitalWrite(relay_pin, HIGH);       // sets the digital pin 13 on
  delay(500);
  digitalWrite(relay_pin, LOW);       // sets the digital pin 13 on
  delay(500);
}  
