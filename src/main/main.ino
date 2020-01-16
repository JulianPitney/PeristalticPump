/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 modified 7 Nov 2016
 by Arturo Guadalupi

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

*/
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <ClickEncoder.h>
#include <TimerOne.h>
#include <LiquidCrystal.h>
#include <Stepper.h>
#include <LiquidCrystal.h>
#define HALFSTEP 8 
#define WITH_LCD 1
//#define LCD_CHARS   16
//#define LCD_LINES    2

//#ifdef WITH_LCD
//void displayAccelerationStatus() {
  //lcd.setCursor(0, 1);  
  //lcd.print("Acceleration ");
 //lcd.print(encoder->getAccelerationEnabled() ? "on " : "off");
//}
//#endif


const int rs = 8, en = 9, d4 = 13, d5 = 12, d6 = 11, d7 = 10;
AccelStepper myStepper(HALFSTEP, 4, 5, 6, 7);
Stepper myStepper(stepsPerRevolution, 4, 5, 6, 7);
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
ClickEncoder *encoder;
int last=0;
int value=0;
int motorSpeed=0;
float steps=0;
float Revs =0;
unsigned long previousMillis=0;
const long interval=3000;
int stepCount = 0;  // number of steps the motor has taken
const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution




void timerIsr() {
  encoder->service();
}


void setup() {
  
  lcd.begin(16, 2);
  encoder = new ClickEncoder(A1, A0, A2);
  myStepper.setAcceleration(80000.0);
  myStepper.setMaxSpeed(850);

//#ifdef WITH_LCD
  //lcd.begin(LCD_CHARS, LCD_LINES);
  //lcd.clear();
  //displayAccelerationStatus();
  //#endif

  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr); 
}




void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  //lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  //lcd.print(millis() / 1000);
  // read the sensor value:
  int sensorReading = analogRead(A0);
  // map it to a range from 0 to 100:
  motorSpeed = map(sensorReading, 0, 1023, 0, 37);

  unsigned long currentMillis=millis();
  
  // set the motor speed:
  if (motorSpeed > 0) 
  {
    myStepper.setSpeed(motorSpeed);
    // step 1/100 of a revolution (one step):
    myStepper.step(stepsPerRevolution / 200);
    

    steps = motorSpeed*200/60;
  
    if (currentMillis-previousMillis>interval) 
    {
      Revs += steps/200*(currentMillis-previousMillis)/1000*0.27;
      previousMillis=currentMillis;  
    }
  }
  
  static int oldReading=0;

  if (sensorReading != oldReading) 
  {
    oldReading = sensorReading;
    lcd.setCursor(0,0);
    lcd.print(motorSpeed*0.27  );
    lcd.print("ml/min     ");
  }
  
  lcd.setCursor(0,1);
  lcd.print(Revs);
  lcd.print("ml   ");
    //lcd.setCursor(10,1);
    //lcd.print(steps);

}
