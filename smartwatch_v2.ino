/*
Smartwatch V2 by Zoltan "strogg" Dancsok dancosk.zoltan@outlook.com

Hardware items
- ssd1306 oled screen i2c
- gí-91 multisensor module i2c
- MAX30100-M blood oxygen and pulse meter i2c
- tp battery charger.
- piezzo 
- usb slot

Function list:
- read temp,humidiy,altitude, accelometer, compass, pulse,bloodO2
- select screens: - welcome screen
                  - Default screen: battery lvl, shooting cross, mission time (sys uptime), bt status, compass circle, gyro cyrcle
                  - Sensor screen: alt,temp,hum,gíro,etc... 
                  - Health: bpm data, blood02 data, pulse animation
                  - ALL  bt,mail,sms,call, status, sight, write o2, bpm anim, compass+gyro cycle
                  - Debug: write all sensor data

Eqv: 
- temp 30°+accel >3  = water warning
- temp > 32° = high temperature warn
- hum  >70%  = high hum. warn. 
- hum <5 = cold warn.
- hum < 0 = ice warn.

- Hour > 11 & <12 = eat warn.
- Hour > 9 & <10  = eat warn.
- Hour > 18 & <20  = eat warn.


*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "strings.h"
#include <SoftwareSerial.h>

SoftwareSerial usb_in(3,4);

int t_back     = A1;
int t_next     = A2;
int brain      = A3;
int piezo      = 2;

int modes      = 0;
int screens    = 0; // 0-4 
int timer      = 0;
int hours      = 0;
int minutes    = 0;
int secs       = 0;
int stepper    = 0;
int bt_status  = 0;        
int bloodO2    = 0;
int pulse      = 0;
int warn_time  = 3000; // warning strings screen time in ms
int piezo_time = 1000; // piezzo sound time in ms

int gir_x      = 0;
int git_y      = 0;
int gir_z      = 0;

int acc_x      = 0;
int acc_y      = 0;
int acc_z      = 0;

int comp_x     = 0;
int comp_y     = 0;
int comp_z     = 0;

float temp     = 0.00;
float hum      = 0.00;
float altitude = 0.00;

String bt_msg  = "";
String lastValue;

char blueToothVal;  


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define GREEN 0x07E0
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  usb_in.begin(9600);
  pinMode(piezzo,OUTPUT);
  
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
 
    
}

 display.mirrorScreen();
 display.setBrightness(252);

welcome();
}


void loop() {
  // put your main code here, to run repeatedly:
timer++;

}

//############################## DISPLAYS ####################################

void display_sight(void){
  display.clearDisplay(); //128x64
//*********animations
if(stepper >=1){ display.fillCircle(2,43, 1, WHITE); }
if(stepper >=2){ display.fillCircle(2,33, 1, WHITE); }
if(stepper >=3){ display.fillCircle(2,23, 1, WHITE); }
if(stepper >=4){ display.fillCircle(2,13, 1, WHITE); }
//**********************
  
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(1,0);             // Start at top-left corner

  display.fillCircle(78, 32, 1, WHITE);
  display.fillCircle(92, 32, 1, WHITE);
  display.fillCircle(106, 32, 1, WHITE);
  display.fillCircle(display.width() / 2, display.height() / 2, 2, WHITE); // 64x32
  display.fillCircle(50, 32, 1, WHITE);
  display.fillCircle(36, 32, 1, WHITE); 
  display.fillCircle(22, 32, 1, WHITE);

  display.fillCircle(64,  6, 1, WHITE);
  display.fillCircle(64, 18, 1, WHITE);
  display.fillCircle(64, 46, 1, WHITE);

   display.drawLine(0, 0, 0, 10, WHITE); // |
   display.drawLine(0, 0, 10, 0, WHITE); //-
   display.setCursor(15,0);
 
 /* if(bt_status == 1){
  display.println(String(message_str[1])); 
  } */

//Serial.println(String(bt_status));
   
   display.drawLine(117, 0, 127, 0, WHITE); //-

   display.drawLine(0, 53, 0, 63, WHITE); // |
   display.drawLine(0, 63, 10, 63, WHITE); //- 
  
   display.drawLine(117, 63, 127, 63, WHITE); // |
   display.drawLine(127, 0, 127, 63, WHITE); // |

    //display.drawLine(124, 10, 127, 10, WHITE); 
    display.setCursor(100,1);
    display.println("05");

    display.drawLine(124, 20, 127, 20, WHITE); 
    display.setCursor(100,18);
    display.println("1");
    
    display.drawLine(124, 40, 127, 40, WHITE); 
    display.setCursor(100,38);
    display.println("3"); 
display.setCursor(20,55);

if(bt_msg == ""){
display.println("Mission:"+String(hours)+":"+String(minutes)+":"+String(secs));
}  else {
display.println(bt_msg);  
}

    display.display(); 
    delay(1);
//*****************
  
} // Sight END

void welcome(void) {
   display.clearDisplay();
   display.setTextColor(WHITE);
   display.setCursor(25,5);
   display.setTextSize(1);
   display.println("SMART GLASS");
   display.setCursor(25,20);
   display.setTextColor(WHITE);
   display.println("v.2.0.1");
   display.setCursor(25,30);
   display.println("by Strogg");
   display.setCursor(12,40);
   display.println("dancsok.zoltan@outlook.com"); 
   display.display(); 
   delay(5000);
 }
