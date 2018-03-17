#include <OneWire.h>
#include <DallasTemperature.h>


#define ONE_WIRE_BUS 7
#define ONE_WIRE_BUS_2 8
#define fadePin 3
#define STARKPin 13

OneWire oneWire(ONE_WIRE_BUS);
OneWire oneWire2(ONE_WIRE_BUS_2);

DallasTemperature sensors(&oneWire);
DallasTemperature sensors2(&oneWire2);

void setup() {
  Serial.begin(9600);
  sensors.begin();
  pinMode(fadePin, OUTPUT);
  pinMode(STARKPin, OUTPUT);
  
}

float checktemp() {
  sensors.requestTemperatures();
  return(sensors.getTempCByIndex(0));
}

float checktemp2() {
  sensors2.requestTemperatures();
  return(sensors2.getTempCByIndex(0));
}

float temperatura;
float temperatura2;
float limite;
float ventana;

void loop() {
  limite = 40.5 ; // ACA DECLARO LIMITE DE TEMPERATURA
  ventana = 5 ;  // ACA DECLARO LA VENTANA 
  temperatura = checktemp();
  temperatura2 = checktemp2();
  Serial.print("Temperature is: ");
  Serial.print(sensors.getTempCByIndex(0)); // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire
  Serial.print(" Temperature 2 is: ");
  Serial.println(sensors2.getTempCByIndex(0)); 
  //Update value every 1 sec.
  delay(500);

  // Sube la tension del pin 3
  for(int i = 0; i<255; i++){
    analogWrite(fadePin, i); delay(1); }
  
  // Baja la tension del pin 3  
  for(int j = 0; j<255; j++){
    int bajar = 255 - j;
    analogWrite(fadePin, bajar); delay(1); }

  if (temperatura >= limite) { digitalWrite(STARKPin , LOW)    ; }
  else if (temperatura < limite - ventana){  digitalWrite(STARKPin, HIGH)  ;}

}  
