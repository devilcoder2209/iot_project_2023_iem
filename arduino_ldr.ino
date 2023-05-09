#include <Wire.h> 
#include <LiquidCrystal_I2C.h> // I2C SCANNER CLASS IS USED TO FIND THE ADRESS OF LCD DISPLAY AND HERE IT IS MENTIONED BELOW

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

//set pin numbers
//const won't change
const int ledPin = 7;   //the number of the LED pin
const int ldrPin = A0;  //the number of the LDR pin
int relay = 8;
//int buzzer = 9;


void setup() {

  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);  //initialize the LED pin as an output
  pinMode(ldrPin, INPUT);   //initialize the LDR pin as an input
  pinMode(relay, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.clear();

}

void loop() {

  int ldrStatus = analogRead(ldrPin);   //read the status of the LDR value

 
   Serial.println(ldrStatus);
   if (ldrStatus <=50) {
    digitalWrite(ledPin, HIGH);
    digitalWrite(relay, LOW);               //turn LED on
    Serial.println("LDR : DARK");
    lcd.clear();

    lcd.setCursor(0,0);
    lcd.print("LDR:DARK,LED:ON");
    lcd.setCursor(1, 1);
    lcd.print(ldrStatus);
  //   tone (buzzer,1000,200);

    delay (1000);
   }
  else {

    digitalWrite(ledPin, LOW);          //turn LED off
    digitalWrite(relay, HIGH); 
    Serial.println("---------------");
    lcd.setCursor(0,0);

    lcd.clear();

    lcd.print("LDR:GLOW,LED:OFF ");
    
    lcd.setCursor(1, 1);
    lcd.print(ldrStatus);

    delay (1000);
  }
}