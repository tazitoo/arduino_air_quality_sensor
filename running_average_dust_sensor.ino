#include <LiquidCrystal.h>
LiquidCrystal lcd(2, 3, 4, 5, 6, 7); // Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7)

const int numReadings = 10;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int aveDust = 0;                // the average

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
  //lcd.print("dust (ug/m^3):");
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

  // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = dustDensity;
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  aveDust = total / numReadings;


  Serial.println(dustDensity); // unit: ug/m3

  lcd.setCursor(0,0);
  //lcd.print(dustDensity); 
  lcd.print(aveDust);

  //delay(1000); 

  lcd.setCursor(0,1);
  lcd.print("Air Qual:");
  lcd.setCursor(10,1);
  if (dustDensity >= 3000) {
    lcd.print("V POOR");
  }
  else if (aveDust >= 1050) {
    lcd.print("poor");
  }
  else if (aveDust >= 300) {
    lcd.print("fair");
  }
  else if (aveDust >= 150) {
    lcd.print("good");
  }
  else if (aveDust >= 0) {
    lcd.print("ex!");
  }

  //  lcd.clear(); // Clears the display 
  //lcd.blink(); //Displays the blinking LCD cursor 

  delay(1000);
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




