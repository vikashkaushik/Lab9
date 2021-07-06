/*
 * Project Lab9
 * Description: Uisng all the concepts, lowSet and highSet value controls the leds. Oled and blynk are also used.
 * Author: Vikash Kaushik
 * Date:7/5/21
 */
SYSTEM_THREAD(ENABLED);
#include <blynk.h>
#include <Wire.h>
#include "SparkFun_VCNL4040_Arduino_Library.h"
#include "oled-wing-adafruit.h"
OledWingAdafruit display;
VCNL4040 proximitySensor;
// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.
  pinMode(D3,INPUT);    // setup button
  pinMode(A4, INPUT);   // tmp36
  pinMode(A5,INPUT); // setup potentiometer
  pinMode(D7,OUTPUT); //led
  pinMode(D6,OUTPUT); //led
  pinMode(D5,OUTPUT); //led
  display.setup();
  display.clearDisplay();
  display.display();
  Serial.begin(9600); 
  Blynk.begin("I5S3IEzBg_rLSDyArIkR2FpW-hgpONn6", IPAddress(167, 172, 234, 162), 9090); 
  Wire.begin();
  proximitySensor.begin(); //Initialize the sensor  
  proximitySensor.powerOnAmbient(); //Power up the ambient sensor

}
uint16_t transitionCount = 0;  // num of transitions
uint16_t lowSet = 0;  // low set 
uint16_t highSet = 0;  // high set
uint16_t numTimesButton = 1;  // numTimes of press button
uint16_t color = 0;  // color tracker
// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // The core of your code will likely live here.
  Blynk.run();
  display.loop();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  
   



  uint64_t reading = analogRead(A4);
  double voltage = (reading * 3.3) / 4095.0;
  double temperature = (voltage - 0.5) * 100;
  display.clearDisplay();
  display.setCursor(0,0);
  display.print(temperature); // prints temp in C
  display.println(" C");
  display.print((temperature)*9/5+32); // prints temp in F
  display.println(" F");
  display.display();  // cause the display to be updated
  //Serial.println(temperature); 
  //Serial.println((temperature)*(9/5)+32); 
  Blynk.virtualWrite(V1,temperature);
  Blynk.virtualWrite(V2,(temperature)*9/5+32);



  unsigned int ambientValue = proximitySensor.getAmbient(); // ambient light level
  

  // potentiometer
  Serial.begin(9600); 
  // it will read the value and print it in the console
  bool value; // button true or false
  value = digitalRead(D3);  // reads the button either true or false
  //Serial.println(value); 
  Serial.println(numTimesButton);
  if (value == true && numTimesButton % 2 != 0) {
    lowSet= analogRead(A5); // if the button is pressed the low set will  be set
    //Serial.println(analogRead(A5));
    numTimesButton++;
    delay(500);
    
  } 
  else if (value == true && numTimesButton % 2 == 0) {
    highSet= analogRead(A5); // if the button is pressed the high set will  be set
    numTimesButton++;
    delay(500);
  } 
  uint16_t temp = lowSet; 
  // finding low set and high set
  if(lowSet>highSet){
    lowSet = highSet;
    highSet = temp;
  }
  //Serial.println(lowSet); 
  //Serial.println(highSet); 


  

  Serial.println(lowSet); 
  Serial.println(highSet);
  Serial.println(ambientValue); 
  
  //Serial.println(proxValue);
  // blue ambientValue below lowset
  if(ambientValue <= (lowSet))
  {
    transitionCount++;
    digitalWrite(D7, HIGH);
    digitalWrite(D6, LOW);
    digitalWrite(D5, LOW);
    //notify transition
    if(transitionCount>1 && color != 1){
      Blynk.notify("Hey, got a push notification!"); // sends notification to Blynk app
    }
    color = 1; // blue
    
  }
  // orange ambientValue between high and low set
  if(ambientValue > (lowSet) && ambientValue<highSet)
  {
    transitionCount++;
    digitalWrite(D6, HIGH);
    digitalWrite(D5, LOW);
    digitalWrite(D7, LOW);

    //notify transition
    if(transitionCount>1 && color != 2){
      Blynk.notify("Hey, got a push notification!"); // sends notification to Blynk app
    }
    color = 2; // orange
  }
  //green ambientValue higher than hughB set
  if(ambientValue > (highSet))
  {
    transitionCount++;
    digitalWrite(D5, HIGH);
    digitalWrite(D6, LOW);
    digitalWrite(D7, LOW);
    
    //notify transition
    if(transitionCount>1 && color != 3){
      Blynk.notify("Hey, got a push notification!"); // sends notification to Blynk app
    }
    color = 3; // green
  }
  // if button c is pressed, find light level
  if(display.pressedC()) {
    display.clearDisplay();

    display.print("High Level: ");
    display.println(highSet);
    display.display(); 
    delay(5000);
  }
  
  // if button a is pressed, find light level
  if (display.pressedA()) {
    display.clearDisplay();

    display.print("Low Level: ");
    display.println(lowSet);
    display.display(); 
    delay(5000);
  }
  
  
}
