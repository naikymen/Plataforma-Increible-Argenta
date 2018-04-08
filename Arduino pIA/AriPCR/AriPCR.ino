//@@ -0,0 +1,63 @@
#include <OneWire.h> 
#include <DallasTemperature.h> 
 
//Termometros 
#define ONE_WIRE_BUS 4 
#define ONE_WIRE_BUS_2 7 

//Ventilador
#define Vent 8
//Calentar
#define Cale 9
//Enfriar
#define Frio 10

// Esto quedo del codigo viejo, se puede sacar
#define fadePin 3 
#define STARKPin 13 

//Inicializa los pins de termometro 
OneWire oneWire(ONE_WIRE_BUS); 
OneWire oneWire2(ONE_WIRE_BUS_2); 
DallasTemperature sensors(&oneWire); 
DallasTemperature sensors2(&oneWire2); 
 
void setup() { 
  Serial.begin(9600); 
  sensors.begin(); 
  pinMode(Vent, OUTPUT); 
  pinMode(Cale, OUTPUT);
  pinMode(Frio, OUTPUT); 
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

float vent = 5;
float temp;

void calentar(int ti){
  temp=checktemp();
  Serial.print("Temperature is: ");
  Serial.println(temp); 
  Serial.print("Calentando a: ");
  Serial.println(ti);       
  while(temp < ti ){
    digitalWrite(Cale, HIGH);
    delay(1000);
    temp=checktemp();

    }
    digitalWrite(Cale, LOW);
    }

void enfriar(int ti){
  temp=checktemp();
  Serial.print("Temperature is: ");
  Serial.println(temp); 
  Serial.print("Enfriando a: ");
  Serial.println(ti);    
  while(temp > ti ){
    digitalWrite(Frio, HIGH);
    delay(1000);
    temp=checktemp();
    }
    digitalWrite(Frio, LOW);
    }    


double mantenert(int ti, int zi){
  int z = 0;
  temp=checktemp();
  while (z < zi) {
    if      (temp < ti-vent) {calentar(ti);
    }
    else if (temp > ti+vent) {enfriar(ti);
    } 
    z = z + 5 ;  // ACA PONER EL TIEMPO QUE TARDARIA EN UN CICLO
    temp=checktemp();
    }
    
  }
    
 
void loop() { 
  mantenert(60,10000);
  mantenert(30,10000);
 }   
