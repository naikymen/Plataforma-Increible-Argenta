#include <DallasTemperature.h>
#include <math.h>         //loads the more advanced math functions
 
void setup() {            //This function gets called when the Arduino starts
  Serial.begin(115200);   //This code sets up the Serial port at 115200 baud rate
  // DEFINIR PUERTOS ACTIVAR MODOS
}
 
double Thermister(int RawADC) {  //Function to perform the fancy math of the Steinhart-Hart equation
 double Temp;
 Temp = log(((10240000/RawADC) - 10000));
 Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
 Temp = Temp - 273.15;              // Convert Kelvin to Celsius
 Temp = (Temp * 9.0)/ 5.0 + 32.0; // Celsius to Fahrenheit - comment out this line if you need Celsius
 return Temp;
}calentar

double calentar(int ti){
  int val;                //Create an integer variable
  double temp;            //Variable to hold a temperature value
  val=analogRead(0);      //Read the analog port 0 and store the value in val
  temp=Thermister(val);   //Runs the fancy math on the raw analog value
  while(temp < ti){
    // PRENDER DOS PELTIER
    }
    }

  
void enfriar(){
  // PRENDER UNA PELTIER EN REVERSA
  // PRENDER UN COOLER 
    }

double mantenert(int ti, int zi){
  int z;
  z = 0;
  while (z < zi) {
    if      (t < ti) {calentar}
    else if (t > ti) {enfriar} 
    z = z + 5  // ACA PONER EL TIEMPO QUE TARDARIA EN UN CICLO
    }
    
  }
 
void loop() {             //This function loops while the arduino is powered
  int val;                //Create an integer variable
  double temp;            //Variable to hold a temperature value
  val=analogRead(0);      //Read the analog port 0 and store the value in val
  temp=Thermister(val);   //Runs the fancy math on the raw analog value
  Serial.println(temp);   //Print the value to the serial port
  delay(1000);            //Wait one second before we do it again
}
