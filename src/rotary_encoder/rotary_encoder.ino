#include <AccelStepper.h>
#include <MultiStepper.h>
#define HALFSTEP 8 

#define WITH_LCD 1

#include <ClickEncoder.h>
#include <TimerOne.h>


#include <LiquidCrystal.h>

AccelStepper myStepper(HALFSTEP, 4, 5, 6, 7);

const int rs = 8, en = 9, d4 = 13, d5 = 12, d6 = 11, d7 = 10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//#define LCD_CHARS   16
//#define LCD_LINES    2

ClickEncoder *encoder;
int last=0;
int value=0;

void timerIsr() {
  encoder->service();
}

//#ifdef WITH_LCD
//void displayAccelerationStatus() {
  //lcd.setCursor(0, 1);  
  //lcd.print("Acceleration ");
 //lcd.print(encoder->getAccelerationEnabled() ? "on " : "off");
//}
//#endif

void setup() {
  //Serial.begin(9600);
  lcd.begin(16,2);
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
float steps=0;
float Revs =0;

unsigned long previousMillis=0;
const long interval=3000;

void loop() {  
  value += encoder->getValue();
  if (850>=value>=0){
    myStepper.setSpeed(value);
    myStepper.run();
    
  if ((value != last) && (850>value>0)) {
    last = value;
    lcd.setCursor(0,0);
    lcd.print(value/2*60*0.00123  );
    lcd.print("ml/min     ");
    
 
  
  }
  
  }
unsigned long currentMillis=millis();

steps = value*400/60;
  
  if ((currentMillis-previousMillis>interval) && (value>0)) {
    Revs += value/2*(currentMillis-previousMillis)/1000*0.00123;
    previousMillis=currentMillis;
  
lcd.setCursor(0,1);
lcd.print(Revs);
lcd.print("ml   ");
  }
}


