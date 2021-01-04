/* Last Edit: 04/01/2021 */

/*-----------------------------2020---------------------------------------
  Mini Tricorder with:
  4D systems uOLED-96-G2
  Arduio Nano3
  MPR121 - 12 way Capacitive Input
  BMP180 - Barometric Presure,Humidity, Temperature
  UVSI1145 -UV/IR Light Intnsity Sensor & Spectroscopy
  TMP007 - IR Thermopile Wireless Temperature Sensor
  MAG3110 - Magnetometer
  3.7v-5v3a Power Converter
  250mah Lithium Battery
  Wireless Charging + Internal USB Charger 5V
  -----------------------------------------------------------------------*/

  

#define DisplaySerial Serial
#include "Seeed_BME280.h"
#include <Wire.h>
#include "Goldelox_Serial_4DLib.h"
#include "Goldelox_const4D.h"
#include "Adafruit_MPR121.h"                     // Capacitive Touch Sensor
#include "Adafruit_SI1145.h"
#include "Adafruit_TMP007.h"


Adafruit_MPR121 cap = Adafruit_MPR121();      // Capacitive Sensor
BME280 bme280;
Adafruit_SI1145 uv = Adafruit_SI1145();
Adafruit_TMP007 tmp007;
Goldelox_Serial_4DLib Display(&DisplaySerial);
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

void setup() {
    Display.Callback4D = mycallback ;
    Display.TimeLimit4D   = 5000 ;
    DisplaySerial.begin(9600);
    Serial.begin(9600); 
    cap.begin(0x5A);                        //       I2C Address: 0x5A
    cap.setThreshholds(12, 6);
 tmp007.begin(TMP007_CFG_1SAMPLE);       //Starts TMP007                                I2C Address: 0x47
  
  
  delay (3000); //let the display start up
     
  Display.gfx_ScreenMode(LANDSCAPE);  // FIRST CMD - Sets Screen Rotation  
  Display.SSTimeout(0) ;
  Display.SSSpeed(0) ;
  Display.SSMode(0) ;
  Display.gfx_Cls();              //Clears the screen

   if (!bme280.init()) {
        Display.println("BME280 Error");
    }
   if (! uv.begin()) {
    Display.println("Si1145 Error");
    while (1);
  }
  if (! tmp007.begin()) {
    Display.println("TMP007 Error");
    while (1);
  }

  
  Display.txt_MoveCursor(0, 0);
  Display.txt_FGcolour(GREEN);                              // change text to GREEN
  Display.putstr("-------------") ;
  Display.txt_MoveCursor(7, 0);
  Display.txt_FGcolour(GREEN);                              // change text to GREEN
  Display.putstr("-------------") ;
  Display.txt_MoveCursor(3, 0) ;    
  Display.txt_FGcolour(PURPLE);                        // change text to BLUE
  Display.putstr("Loading") ;
  Display.txt_MoveCursor(4, 0);
  Display.txt_FGcolour(GREEN);  
  Display.putstr(".");  
  delay(100);
  Display.putstr(".");   
  delay(100);
  Display.putstr(".");  
  delay(100); 
  Display.putstr(".");  
  delay(200);
  Display.putstr("Complete");  
  delay(500);
    Display.gfx_Cls();                                                                       //Clears Screen
  Display.txt_MoveCursor(0, 0); Display.txt_FGcolour(ORANGE);                              // change text to ORANGE
  Display.putstr("-------------") ;    
  Display.txt_MoveCursor(1, 0) ;
  Display.txt_FGcolour(BLUE);
  Display.println("  Tricorder");    //13x7 Display
  Display.txt_FGcolour(BLUE);
  Display.println("    mk.II");    //13x7 Display
  Display.txt_MoveCursor(3, 0) ;Display.txt_FGcolour(PURPLE);                           // change text to PURPLE
  Display.putstr("   Select") ;
  Display.txt_MoveCursor(4, 0);
  Display.putstr(" Scan Mode");                       
  Display.txt_MoveCursor(7, 0); Display.txt_FGcolour(ORANGE);                              // change text to ORANGE
  Display.putstr("---˅--˅--˅---") ;   
  Display.txt_MoveCursor(1, 0) ;
  Display.txt_FGcolour(BLUE);
    
}

void loop() { 
  
  currtouched = cap.touched();

  
if (cap.touched() & (1 << 1))                                 //Button 1 - Barometer
  {
    Baro();
  }

if (cap.touched() & (1 << 2))                                 //Button 2 - Light
  {
    Light();
  }

if (cap.touched() & (1 << 3))                                 //Button 3 - IR
  {
    IRT();
  }
  
if (cap.touched() & (1 << 0))                                 //Button 0 - About
  {
    About();
  }

if (cap.touched() & (1 << 4))                                 //Button 4 - BLANK
  {
    //blank
  }
  
if (cap.touched() & (1 << 5))                                 //Button 5 - BLANK
  {
    //blank
  }
  
if (cap.touched() & (1 << 6))                                 //Button 6 - BLANK
  {
   //blank
  }
  

  delay(100);
  
}

void Baro()
{
  Display.gfx_Cls();                                                                       //Clears Screen
  Display.txt_MoveCursor(0, 0); Display.txt_FGcolour(ORANGE);                              // change text to ORANGE
  Display.putstr("--Barometer--") ;                         
  Display.txt_MoveCursor(7, 0); Display.txt_FGcolour(ORANGE);                              // change text to ORANGE
  Display.putstr("-------------") ;   
  Display.txt_MoveCursor(1, 0) ;
  Display.txt_FGcolour(PURPLE);
  
    float pressure;
    //get and print temperatures
    Display.putstr("Temp: ");
  Display.txt_FGcolour(GREEN);  
    Display.print(bme280.getTemperature());
  Display.txt_FGcolour(RED);
    Display.println(" C");//The unit for  Celsius because original arduino don't support special symbols
    //get and print atmospheric pressure data    
  Display.txt_FGcolour(PURPLE);
    Display.println("Pressure: ");
  Display.txt_FGcolour(GREEN);  
  Display.putstr("  ");  
    Display.print(pressure = bme280.getPressure());
  Display.txt_FGcolour(RED);
    Display.println(" Pa");
    //get and print altitude data
  Display.txt_FGcolour(PURPLE);
    Display.println("Altitude: ");
  Display.txt_FGcolour(GREEN);  
  Display.putstr("    ");  
    Display.print(bme280.calcAltitude(pressure));
  Display.txt_FGcolour(RED);
    Display.println("  m");
    //get and print humidity data
  Display.txt_FGcolour(PURPLE);
    Display.print("Humidity: ");
  Display.txt_FGcolour(GREEN); 
    Display.print(bme280.getHumidity());
  Display.txt_FGcolour(RED);
    Display.println(" %");

    delay(1500);
    Baro();           //test repeat function
}


void Light()
{
  Display.gfx_Cls();                                                                       //Clears Screen
  Display.txt_MoveCursor(0, 0); Display.txt_FGcolour(ORANGE);                              // change text to ORANGE
  Display.putstr("----Light----") ;                         
  Display.txt_MoveCursor(7, 0); Display.txt_FGcolour(ORANGE);                              // change text to ORANGE
  Display.putstr("-------------") ;   
  Display.txt_MoveCursor(1, 0) ;
  Display.txt_FGcolour(PURPLE);
  int UVindex; char UVI;
  UVindex = uv.readUV();
  // the index is multiplied by 100 so to get the
  // integer index, divide by 100!
  UVindex /= 100.0;
  itoa(UVindex, UVI, 10);
  delay(100);
  Display.print("UV:         /n");
  Display.txt_FGcolour(GREEN);  
  Display.print(UVI);    
  Display.txt_MoveCursor(2, 0) ;
  Display.txt_FGcolour(PURPLE);
  Display.print("IR:         /n");         //IR
  Display.txt_FGcolour(GREEN);  
  Display.print(uv.readIR());    
  Display.txt_MoveCursor(3, 0) ;
  Display.txt_FGcolour(PURPLE);
  Display.print("VisLight    /n");     //Visible Light
  Display.txt_FGcolour(GREEN);  
  Display.print(uv.readVisible());
  delay(100);
}


void IRT()
{
  Display.gfx_Cls(); 
  Display.txt_MoveCursor(0, 0); Display.txt_FGcolour(ORANGE);                              // change text to ORANGE
  Display.putstr("-----IRT-----") ;                         
  Display.txt_MoveCursor(7, 0); Display.txt_FGcolour(ORANGE);                              // change text to ORANGE
  Display.putstr("-------------") ;    
  Display.txt_MoveCursor(1, 0) ;
  Display.txt_FGcolour(PURPLE);
 float objt = tmp007.readObjTempC();
   Display.println("ObjTemp:");
  Display.txt_FGcolour(GREEN);  
  Display.putstr("       /n");  
   Display.print(objt); 
  Display.txt_FGcolour(RED);
   Display.println(" C");
   float diet = tmp007.readDieTempC();
  Display.txt_FGcolour(PURPLE);
   Display.println("DieTemp:");
  Display.txt_FGcolour(GREEN);    
  Display.putstr("       /n");  
   Display.print(diet); 
  Display.txt_FGcolour(RED);
   Display.println(" C");

  
  delay(100);
}

void About()
{
  Display.gfx_Cls(); 
  Display.txt_MoveCursor(0, 0); Display.txt_FGcolour(ORANGE);                              // change text to ORANGE
  Display.putstr("----About----") ;                         
  Display.txt_MoveCursor(7, 0); Display.txt_FGcolour(ORANGE);                              // change text to ORANGE
  Display.putstr("---Dan 2020---") ;    
  Display.txt_MoveCursor(1, 0) ;
  Display.txt_FGcolour(BLUE);
  Display.println("  Tricorder");    //13x7 Display
  Display.txt_FGcolour(BLUE);
  Display.println("    mk.II");    //13x7 Display
  Display.txt_FGcolour(PURPLE);     
  Display.txt_MoveCursor(3, 0) ;
  Display.println("uOLED  MPR121"); 
  Display.txt_MoveCursor(4, 0) ;
  Display.println("SI1145 TMP007");
  Display.txt_MoveCursor(5, 0) ;
  Display.println("BMP180 MAG3110");
  Display.txt_MoveCursor(6, 0) ;
  Display.println("250mAh QiCHRG");

  delay(100);
}





void mycallback(int ErrCode, unsigned char Errorbyte)
{
  // Pin 13 has an LED connected on most Arduino boards. Just give it a name
  int led = 13;
  pinMode(led, OUTPUT);
  while(1)
  {
    digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(200);               // wait for 200 ms
    digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
    delay(200);               // wait for 200 ms
  }
}
