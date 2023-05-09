#define BLYNK_TEMPLATE_ID "TMPL3o-aJ_nd3"
#define BLYNK_TEMPLATE_NAME "Home Automation2"
#define BLYNK_AUTH_TOKEN "4uRT5Jrcy5DooinIr4-QCzDyi9_DOPAo"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>  
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>  



#define trig D4
#define echo D5
#define DHTPIN D2 
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
float t, h;


char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Saumyadeep's OnePlus";
char pass[] = "Sau@2022";


//global variable declaration 

int buzzer = D0;
int smokeA0 = A0;
int flag = 0;
int pirValue;                   // Place to store read PIR Value
int pinValue; 
int relay = D3;

BlynkTimer timer;
//Defining Macro for all analog and digital pins

#define Buzzer D0
#define pirPin D6

int flame = D1;

//Blynk write

BLYNK_WRITE(V3) {
  pinValue = param.asInt();
}

// here setup code takes the input and output

void setup(){

  //Input Output

  pinMode(smokeA0, INPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(pirPin, INPUT);
  pinMode(flame, INPUT);
  pinMode(relay,OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  dht.begin();
  Serial.begin(115200);
  
  //Blynk Auth  
  
  Blynk.begin(auth, ssid, pass);

  //Function Call

  timer.setInterval(100L, getPirValue);
  timer.setInterval(100L, smokeSensor); 
  timer.setInterval(100L, fireSensor);
  timer.setInterval(100L, DHT11sensor);
  timer.setInterval(100L, ultrasonic); 
}


void DHT11sensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V7, t);
  Blynk.virtualWrite(V6, h);
}


BLYNK_WRITE(V12) {
 bool Relay = param.asInt();
  if (Relay == 1) {
    digitalWrite(relay, HIGH);
  } else {
    digitalWrite(relay, LOW);
  }
}

void getPirValue(void)        //Get PIR Data
  {
   pirValue = digitalRead(pirPin);
    if (pirValue == HIGH) 
     { 
      tone (Buzzer,1000,200);
       Serial.println("Motion detected");
       Blynk.logEvent("motionpir");  
     }
     else{
       Serial.println("No Motion detected");
     }
  }


void smokeSensor(){   //function for smoke sensor
 
  int data = analogRead(smokeA0);
  Serial.print("Pin A0: ");
  Serial.println(data);


  if(data > 440)     // Change the Trashold value
  {
    Blynk.logEvent("gassensor");
  }
  Blynk.virtualWrite(V2, data);
}

void fireSensor(){ 
   int isButtonPressed = digitalRead(flame);
  if (isButtonPressed==0 && flag==0) {
    Serial.println("Fire in the House");  
    Blynk.logEvent("fire_sensor","Fire Detected");
    
    flag=1;
  }
  else if (isButtonPressed==1)
  {
    flag=0;
    
  }
 
}

void ultrasonic() {
  digitalWrite(trig, LOW);
  delayMicroseconds(4);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long tt = pulseIn(echo, HIGH);
  long cm = tt / 29 / 2;

  Blynk.virtualWrite(V4, cm);
}


void loop(){
  Blynk.run();
  timer.run();
}