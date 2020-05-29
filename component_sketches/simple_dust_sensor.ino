#include <LiquidCrystal.h>
LiquidCrystal lcd(2, 3, 4, 5, 6, 7); // Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7)

/*
 Standalone Sketch to use with a Arduino UNO and a
 Sharp Optical Dust Sensor GP2Y1010AU0F
 */

int measurePin = 0; //Connect dust sensor to Arduino A0 pin
int ledPower = 12;   //Connect 3 led driver pins of dust sensor to Arduino D12

int samplingTime = 240;
int deltaTime = 40;
int sleepTime = 9680;

float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

void setup(){
  Serial.begin(9600);
  //Serial.begin(115200);
  pinMode(ledPower,OUTPUT);
  lcd.begin(16,2); // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display } 
//  lcd.print('dust density in ug per m^3:');
}

void loop(){
  digitalWrite(ledPower,LOW); // power on the LED
  delayMicroseconds(samplingTime);

  voMeasured = analogRead(measurePin); // read the dust value

  delayMicroseconds(deltaTime);
  digitalWrite(ledPower,HIGH); // turn the LED off
  delayMicroseconds(sleepTime);

  // 0 - 5V mapped to 0 - 1023 integer values
  // recover voltage
  calcVoltage = voMeasured * (5.0 / 1024.0);

  // linear eqaution taken from http://www.howmuchsnow.com/arduino/airquality/
  // Chris Nafis (c) 2012
  dustDensity = 170 * calcVoltage - 0.1;


  Serial.println(dustDensity); // unit: ug/m3

  
  lcd.print(dustDensity); 
  lcd.setCursor(0,0);
  delay(1000); 
  lcd.clear(); // Clears the display 
  //lcd.blink(); //Displays the blinking LCD cursor 

  //delay(3000);
}

/*
Air Quality Chart - Small Count Reading (0.5 micron)+
 
 3000 +     = VERY POOR
 1050-3000  = POOR
 300-1050   = FAIR
 150-300    = GOOD
 75-150     = VERY GOOD
 0-75       = EXCELLENT
 */


