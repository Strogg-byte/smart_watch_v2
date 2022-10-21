#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <dht.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>


dht DHT;
#define DHT11_PIN 10
uint32_t delayMS;

SoftwareSerial BTserial(2, 3); //Connect HC-06, RX, TX
char c=' ';
String data; 
int temp = 0;
int hum  = 0;

int analogInput = 0;
float vout = 0.0;
float vin = 0.0;
float R1 = 100000.0; // resistance of R1 (100K)
float R2 = 10000.0; // resistance of R2 (10K)
int value = 0;
int a[5] = {};

int screen = 1;

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);
unsigned long delayTime;


int timer      = 0; // timer value
int headache   = 0;   // headache vibro mode status 0,1


void setup() {
  Serial.begin(9600);
  Serial.println("KIROSHI OPTICALS Opti-Flash Mk1. Booting");
pinMode(analogInput, INPUT);

  BTserial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x64)
  
  display.setTextSize(1);
  display.setTextColor(WHITE);
 
welcome();
//delay(3000);
//main_screen();
}



void loop() { 
timer++;
if(timer==3000){timer=0;}

int chk = DHT.read11(DHT11_PIN);
if(DHT.temperature > -50){temp = DHT.temperature;}
//temp = DHT.temperature ;
if(DHT.humidity > -50){hum  = DHT.humidity;}

vin = 0.0;

value = analogRead(analogInput);
  vout = (value * 5) / 1024.0;
  vin = vout / (R2 / (R1 + R2));
  if (vin < 0.09)
  {
    vin = 0.0;
  }
if(vin  < 1 and !BTserial.available() and !Serial.available() and screen == 2){ main_screen(); screen =1; }

if(vin > 1 and !BTserial.available() and !Serial.available()){ 
  screen = 2;
  vin = vin + 0.3;
  
        display.setTextSize(2);
        display.clearDisplay();
        display.setCursor(25,20); 
       // display.print(String(vin)+ " V");
         display.print(vin);
         display.print(" V");
        display.display();
        Serial.println(vin);
        BTserial.println(vin); 
          
  }

 if (BTserial.available()){  
        c = BTserial.read();
        Serial.write(c);
      if (c != -1) {
      data += c; 
      if (c == '\n') { 
        display.setTextSize(1);
        display.setCursor(1,1);
        display.clearDisplay();
        display.print(data); 
        display.display();
        
        delay(5000);
        data = ""; 
        main_screen();
        
          }
       }
        
    }
 
   
    if (Serial.available()){
        c =  Serial.read();
        BTserial.write(c); 
        Serial.write(c); 
        if (c != -1) {
      data += c; 
      if (c == '\n') { 
        display.setTextSize(1);   
        display.setCursor(1,1);
        display.clearDisplay();
        display.print(data); 
        display.display();
        delay(5000);
        data = ""; 
        main_screen();
        
          }
       }
        
        
    }



//####################### main screen
 if(timer ==500){
  main_screen();
 }

//*****************
//###########################################
}


void welcome(){
 display.clearDisplay();
   display.setCursor(25,0);
   display.println("KIROSHI");
   display.setCursor(25,10);
   display.println("OPTICALS");
   display.setCursor(25,20);
   display.println("1990-2022");
   display.setCursor(12,30);
   display.println("Opti-Flash Mk1."); 
   display.setCursor(25,40);
   display.println("Hud Edition");
   display.display(); 
}

void main_screen(){
 display.setTextSize(1);
 display.clearDisplay();
 display.setCursor(20,0);
 display.print(temp);
display.setCursor(35,0);
display.print("C");
 
 display.setCursor(90,0);
 display.print(hum);
 
 display.setCursor(105,0);
  display.print("%");

//############### figyelmeztetesek
if(temp >=30){display.setCursor(40,0); display.print("!");}
if(hum >=70){display.setCursor(85,0); display.print("!");}


if(temp <= 0){display.setCursor(45,0); display.print("!");}

//#################################
  
  display.drawPixel(78, 32, WHITE);
  display.drawPixel(92, 32,  WHITE);
  display.drawPixel(106, 32, WHITE);
  display.drawPixel(64,32, WHITE); // 64x32
  display.drawPixel(50, 32,  WHITE);
  display.drawPixel(36, 32,  WHITE); 
  display.drawPixel(22, 32, WHITE);
  display.drawPixel(64,  6,  WHITE);
  display.drawPixel(64, 18, WHITE);
  display.drawPixel(64, 46, WHITE);
   display.drawLine(0, 0, 0, 10, WHITE); // |
   display.drawLine(0, 0, 10, 0, WHITE); //-
   display.setCursor(15,0);
   display.drawLine(117, 0, 127, 0, WHITE); //-
   display.drawLine(0, 53, 0, 63, WHITE); // |
   display.drawLine(0, 63, 10, 63, WHITE); //- 
   display.drawLine(117, 63, 127, 63, WHITE); // |
   display.drawLine(127, 0, 127, 63, WHITE); // |
    display.drawLine(124, 20, 127, 20, WHITE); 
    display.drawLine(124, 40, 127, 40, WHITE); 
  display.setCursor(20,55);
    display.display();
 
}
