#include <Adafruit_SSD1306.h>

#include <gfxfont.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <Adafruit_GFX.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 7

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

#include <SPI.h>
#include <Wire.h>
/*
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
*/
// If using software SPI (the default case):
#define OLED_MOSI   9  // SCL
#define OLED_CLK   10  // SDA
#define OLED_DC    11
#define OLED_CS    12  // nada, este display no tiene CS
#define OLED_RESET 13
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

#define LOGO16_GLCD_HEIGHT 64 
#define LOGO16_GLCD_WIDTH  128 

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif


float temp0;
float temp1;
int peltier_status;
int cycle_wait;
float target;
int block = 98;


float checktemp(int onewire_index) {
  sensors.requestTemperatures();
  //return(roundTemp(sensors.getTempCByIndex(onewire_index)));
  return(sensors.getTempCByIndex(onewire_index));
}

float roundTemp(float t){
  return(round(t*10)/10);
}

void t_tapa(float temp0) {
  display.setTextColor(WHITE);
  display.fillRect(xPos(24), yPos(0), xPos(2), 7, 0);
  display.setCursor(xPos(24),yPos(0));
  if (temp0 <= block){digitalWrite(5, HIGH); display.print("+");}
  if (temp0 > block){digitalWrite(5, LOW); display.print("-");}
  display.display();
}

int xPos(int x){
  return(5*x);
}

int yPos(int y){
  return(y*8);
}


float prevTemp1 = checktemp(1);


void setup()   {                
  Serial.begin(9600);
  sensors.begin();

  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);  // Set Pin to LOW to turn Relay OFF
  pinMode(6, OUTPUT);  // OUTPUT significa 5V
  digitalWrite(6, LOW);  // Set Pin to LOW to turn Relay OFF

  pinMode(3, OUTPUT);
  digitalWrite(3, LOW);  // Set Pin to LOW to turn off lid heating
  pinMode(5, OUTPUT);  // OUTPUT significa 5V
  digitalWrite(5, LOW);  // Set Pin to LOW to turn fans off
  
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC);
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  //display.display();
  // delay(2000);

  // Clear the buffer.
  display.clearDisplay();

  // text display tests
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Hello, world!");
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.println(3.141592);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print("0x"); display.println(0xDEADBEEF, HEX);
  display.display();
  delay(300);
  display.clearDisplay();

  // invert the display
  display.invertDisplay(true);
  delay(100); 
  display.invertDisplay(false);
  delay(100); 
  display.clearDisplay();
  delay(100);

  display.display();

  
  temp0 = checktemp(0);
  temp1 = checktemp(1);
  infoUpdate(target, temp0, temp1);

  display.setCursor(xPos(16),yPos(0));  display.print(block); display.print(" C");
  display.display();
  
  while(temp0 < block){
    t_tapa(temp0);
    delay(500);
    temp0 = checktemp(0);

    display.setTextColor(BLACK, WHITE); // 'inverted' text
    display.fillRect(xPos(5), yPos(0), xPos(8), 7, 0);
    display.setCursor(xPos(5),yPos(0));  display.print(temp0);
    display.display();
  }
  display.setTextColor(WHITE);
}



void loop() {

  // La peltier va a desconectarse si le paso un 0 a la temperatura
  target = 0; cycle_wait = 3;
  peltier(target, cycle_wait);

  target = 90; cycle_wait = 5;
  peltier(target, cycle_wait);

  target = 55; cycle_wait = 5;
  peltier(target, cycle_wait);

  //target = 72; cycle_wait = 5;
  //peltier(target, cycle_wait);

  //target = 90; cycle_wait = 5;
  //peltier(target, cycle_wait);

  //peltier_status = 0; cycle_wait = 3;
  //peltier(peltier_status, cycle_wait);

  display.clearDisplay();
}


float infoUpdate(int target, float temp0, float temp1) {
    
  // Dibujar la info
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.fillRect(xPos(0), yPos(0), xPos(24), 16, 0);
  display.println("LID:       ->");
  display.println("BLK:       ->");
  display.setCursor(xPos(16),yPos(0));  display.print(block); display.print(" C");
  display.setCursor(xPos(16),yPos(1));  display.print(target); display.print(" C");

  display.setCursor(xPos(5),yPos(0));
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.print(temp0);  // tapa LID
  display.setCursor(xPos(5),yPos(1));
  display.println(temp1);  // bloque BLK

  display.setTextColor(WHITE);
  display.print("Log: ");
  if(temp1 < 0){display.print("error detected");}

  
  display.setCursor(xPos(0),yPos(3));
  display.print("dT/dt: ");
  display.fillRect(xPos(7), yPos(3), xPos(10), 7, 0);
  display.print(temp1 - prevTemp1);
  
  //Serial.println(temp0 - prevTemp1);
  display.display();
  //display.clearDisplay();

  prevTemp1 = temp1;
}


void cycleWait (int espera, int target_t) {
   int a = 0;
   while( a < espera ) {
      temp0 = checktemp(0);
      temp1 = checktemp(1);

      t_tapa(temp0);
      infoUpdate(target_t, temp0, temp1);
      
      display.fillRect(xPos(19), yPos(3), xPos(8), 7, 0);
      display.setCursor(xPos(19),yPos(3)); display.print("TL: "); display.print(espera - a);
      display.display();

      delay(500);
      
      a++;
   }
   
   display.fillRect(xPos(19), yPos(3), xPos(10), 7, 0);
   display.setCursor(xPos(19),yPos(3)); display.print("TL: "); display.print("-");
   display.display();
}  


void peltier(int t, int tiempo) {
  // 0 -- Nada
  // 1 +- Calentar
  // 2 -+ Enfriar

  temp0 = checktemp(0);  // LID
  temp1 = checktemp(1);  // BLK
  

  int direccion;
  if (t == 0) {direccion = 0;} // Nada
  if (t != 0) {
    if (t < temp1) {direccion = 2;}; // Enfriar
    if (t > temp1) {direccion = 1;} // Calentar
  }

  display.fillRect(xPos(16),yPos(0), xPos(8), 16, 0);

  
  digitalWrite(4, LOW);
  digitalWrite(6, LOW);

  
  if (direccion == 0) {
    // Nada
    display.setCursor(xPos(16),yPos(1));  display.print(t); display.print(" C");
    display.setCursor(xPos(24),yPos(1));  display.print("i");
    display.display();
    
    // Apaga todo y espera la duración del ciclo
    cycleWait(tiempo, 99);
  }

  
  if (direccion == 1) {
    // Calentar
    display.fillRect(xPos(24),yPos(1), xPos(1), 16, 0);
    display.setCursor(xPos(24),yPos(1)); display.print("+");
    display.setCursor(xPos(16),yPos(1));  display.print(t); display.print(" C");
    
    display.display();
    
    digitalWrite(4, HIGH);
    digitalWrite(6, LOW);
    
    while (temp1 < t) {
      temp0 = checktemp(0);
      temp1 = checktemp(1);

      t_tapa(temp0);
      infoUpdate(t, temp0, temp1);
    }
    // Se llama a si misma en modo Nada por la duración del ciclo
    peltier(0, tiempo);
  }

  
  if (direccion == 2) {
    // Enfriar
    display.setCursor(xPos(24),yPos(1)); display.print("-");
    display.setCursor(xPos(16),yPos(1));  display.print(t); display.print(" C");
    display.display();
    
    digitalWrite(4, LOW);
    digitalWrite(6, HIGH);
    
    while (temp1 > t || temp1 < 0) {
      temp0 = checktemp(0);
      temp1 = checktemp(1);

      t_tapa(temp0);
      infoUpdate(t, temp0, temp1);
    }
    // Se llama a si misma en modo Nada por la duración del ciclo
    peltier(0, tiempo);
  }
}
