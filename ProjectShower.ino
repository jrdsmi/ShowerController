/*
 * Created: 10/17/2017
 * Modified: 2/13/2018
 * Author: Jared Smith
 * 
 * Description: Aims to adjust the water temperature to a desirable level.
 */
#include <LiquidCrystal.h>
#include <myStepper.h>
#define MY_TEMP 80 // DEBUG ME
#define MAX_STEPS 1200 // DEBUG ME
int tempPin = 0;
float prevErr = 0;
int prevClock = 0;
float integral = 0;
//                RS E  D4 D5 D6 D7
LiquidCrystal lcd(2, 4, 5, 6, 9, 11);
// initialize the stepper library on pins 8 through 11:
myStepper stepper(200, 8, 10); // DEBUG ME
float pidController(float tempF, float desiredTemp, int currClock);

void setup() {
  lcd.begin(16, 2); //setup the LCD
  delay(100);
}

void loop() {
  int tempReading = analogRead(tempPin);
  // This is OK
  double tempK = log(10000.0 * ((1024.0 / tempReading - 1)));
  tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK )) * tempK );       //  Temp Kelvin
  float tempC = tempK - 273.15;            // Convert Kelvin to Celcius
  float tempF = (tempC * 9.0) / 5.0 + 32.0; // Convert Celcius to Fahrenheit
  /*  replaced
    float tempVolts = tempReading * 5.0 / 1024.0;
    float tempC = (tempVolts - 0.5) * 10.0;
    float tempF = tempC * 9.0 / 5.0 + 32.0;
  */

  if(abs(MY_TEMP - tempF) > 1 && millis() - prevClock > 1000) { // Maximum allowed deviation from desired temp
    stepper.step((int) pidController(tempF, MY_TEMP, millis()));
  }

  
  lcd.setCursor(0, 0);
  //lcd.print("Temp         C  "); // Display Temperature in C

  lcd.print("Temp         F  "); // Display Temperature in F
  lcd.setCursor(6, 0);
 
  //lcd.print(tempC); // Display Temperature in C

  lcd.print(tempF); // Display Temperature in F
  delay(100);
}


// This function will change the position of the motor based on the reading of the temperature. 
// Used in many applications to control temperature.
float pidController(float tempF, float desiredTemp, int currClock) {
  float deltaT = (currClock - prevClock)/1000.0; // time between the last call of this function in seconds
  float err = desiredTemp - tempF; // current deviation from desired temp
  integral = integral + err*deltaT;
  float derivative = (err - prevErr)/deltaT;
  prevErr = err; 
  float out = 1*err + 1*integral + 1*derivative; // DEBUG ME
  prevClock = currClock;
  return out;  //DEBUG ME
}

