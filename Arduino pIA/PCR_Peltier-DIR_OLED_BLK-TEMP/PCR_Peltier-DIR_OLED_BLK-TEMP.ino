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

/*********************************************************************
This is an example for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

This example is for a 128x64 size display using SPI to communicate
4 or 5 pins are required to interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

#include <SPI.h>
#include <Wire.h>
/*
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
*/
// If using software SPI (the default case):
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

#define LOGO16_GLCD_HEIGHT 64 
#define LOGO16_GLCD_WIDTH  128 

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif


void setup()   {                
  Serial.begin(9600);
  sensors.begin();

  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);  // Set Pin to LOW to turn Relay OFF
  pinMode(6, OUTPUT);  // OUTPUT significa 5V
  digitalWrite(6, LOW);  // Set Pin to LOW to turn Relay OFF
  
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
}

float temperatura;
float temperatura_anterior = checktemp();
int peltier_status;
int cycle_wait;

void loop() {
  infoUpdate(99);

  peltier_status = 0; cycle_wait = 10;
  peltier(peltier_status, cycle_wait);

  peltier_status = 1; cycle_wait = 5;
  peltier(peltier_status, cycle_wait);

  peltier_status = 2; cycle_wait = 5;
  peltier(peltier_status, cycle_wait);

  peltier_status = 3; cycle_wait = 1;
  peltier(peltier_status, cycle_wait);

  display.clearDisplay();
}


float checktemp() {
  sensors.requestTemperatures();
  return(sensors.getTempCByIndex(0));
}


int infoUpdate(int temp_target) {
  temperatura = checktemp();
  
  // Dibujar la info
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Block:     Peltier:");
  display.setTextSize(2);
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.print(temperatura);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.print(" Tgt: ");
  display.fillRect(4*24, 8*1, 5*8, 7, 0);
  display.print(temp_target);
  display.println(" C");
  display.println("           Stat:");
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.print("dT/dt: ");
  display.fillRect(4*10, 8*3, 5*8, 7, 0);
  display.println(temperatura - temperatura_anterior);
  //Serial.println(temperatura - temperatura_anterior);
  display.display();
  //display.clearDisplay();

  temperatura_anterior = temperatura;

  return temperatura;
}


void cycleWait (int espera, int target_t) {
   int a = 0;
   while( a < espera ) {
      infoUpdate(target_t);
      display.fillRect(4*22, 8*3, 5*8, 7, 0);
      display.setCursor(4*22,8*3); display.print("TL: "); display.print(espera - a);
      display.display();
      if (target_t != 99) delay(350); // En el futuro debería hacer cosas con tiempos posta...
      if (target_t == 99) delay(350); // En el futuro debería hacer cosas con tiempos posta...
      a++;
   }
   display.fillRect(4*22, 8*3, 5*8, 7, 0);
   display.setCursor(4*22,8*3); display.print("TL: "); display.print("-");
   display.display();
}  


void peltier(int direccion, int tiempo) {
  // 0 -- Nada
  // 1 +- Calentar
  // 2 -+ Enfriar
  // 3 ++ Nada
  
  if (direccion == 0) {
    // Nada
    display.fillRect(4*24, 8*2, 5*8, 7, 0);
    display.setCursor(4*24,8*2);  display.print("  --");
    display.display();
    digitalWrite(4, LOW);
    digitalWrite(6, LOW);
    // Apaga todo y espera la duración del ciclo
    int target_t = 99;
    cycleWait(tiempo, target_t);
  }
  if (direccion == 1) {
    // Calentar
    display.fillRect(4*24, 8*2, 5*8, 7, 0);
    display.setCursor(4*24,8*2);  display.print("  +-");
    display.display();
    digitalWrite(4, HIGH);
    digitalWrite(6, LOW);
    int target_t = 80;
    temperatura = checktemp();
    while (temperatura < target_t) {
      float temperatura = infoUpdate(target_t);
      delay(270);
    }
    // Se llama a si misma en modo Nada por la duración del ciclo
    peltier(0, tiempo);
  }
  if (direccion == 2) {
    // Enfriar
    digitalWrite(4, LOW);
    digitalWrite(6, HIGH);
    display.fillRect(4*24, 8*2, 5*8, 7, 0);
    display.setCursor(4*24,8*2);  display.print("  -+");
    display.display();
    int target_t = 55;
    temperatura = checktemp();
    while (temperatura > target_t) {
      float temperatura = infoUpdate(target_t);
      delay(270);
    }
    // Se llama a si misma en modo Nada por la duración del ciclo
    peltier(0, tiempo);
  }
  if (direccion == 3) {
    // Nada
    digitalWrite(4, HIGH);
    digitalWrite(6, HIGH);
    display.fillRect(4*24, 8*2, 5*8, 7, 0);
    display.setCursor(4*24,8*2);  display.print("  ++");
    display.display();
    // Apaga todo y espera la duración del ciclo
    int target_t = 99;
    cycleWait(tiempo, target_t);
  }
}
