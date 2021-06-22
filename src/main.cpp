#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_AHTX0.h>
#include "ACS712.h"
#include <SoftwareSerial.h>

SoftwareSerial softSerial(6, 7); // RX, TX

/*
---A0  current 12V
---A1  current 5V
A2  relay
---A3  current always on
A4  I2C
A5  I2C
A6  chargeStatePin
---A7  voltage
*/

Adafruit_AHTX0 aht;
ACS712 current12VBus(ACS712_05B, A0);
ACS712 current5VBus(ACS712_05B, A3);
ACS712 currentAlwaysON(ACS712_05B, A1);

const int voltageSensor = A7;

float vOUT = 0.0;
float vIN = 0.0;
float R1 = 30000.0;
float R2 = 7500.0;
int value = 0;

const int relay = A2;
const int fan = 9;
const int limitSwitch = 8; //sebelumnya A0
int switchState = 0;

const int chargeStatePin = A6;
int chargeState = 0;

void setup() {
  Serial.begin(115200);
  aht.begin();
  current12VBus.calibrate();
  current5VBus.calibrate();
  currentAlwaysON.calibrate();

  pinMode(relay, OUTPUT);
  pinMode(fan, OUTPUT);
  digitalWrite(fan, LOW);

  softSerial.begin(9600);
}

void loop() {
  switchState = digitalRead(limitSwitch);
  if (switchState == LOW) {
    digitalWrite(relay, LOW);
    digitalWrite(fan, LOW);
  } else {
    digitalWrite(relay, HIGH);
    digitalWrite(fan, HIGH);
  }

  float current12V = current12VBus.getCurrentDC();
  if (current12V < 0) {
    current12V = 0;
  }
  float current5V = current5VBus.getCurrentDC();
  if (current5V < 0) {
    current5V = 0;
  }
  float currentON = currentAlwaysON.getCurrentDC();
  if (currentON < 0) {
    currentON = 0;
  }
  
  // Send it to serial
  Serial.println(String("Current 12V = ") + current12V + " A");
  Serial.println(String("Current 5V = ") + current5V + " A");
  Serial.println(String("Current ON = ") + currentON + " A");
  softSerial.println(String("Current 12V = ") + current12V + " A");
  softSerial.println(String("Current 5V = ") + current5V + " A");
  softSerial.println(String("Current ON = ") + currentON + " A");

  value = analogRead(voltageSensor);
  vOUT = (value * 5.0) / 1024.0;
  vIN = vOUT / (R2/(R1+R2));
  Serial.print("Voltage Input = ");
  Serial.print(vIN);
  Serial.println(" V");
  softSerial.print("Voltage Input = ");
  softSerial.print(vIN);
  softSerial.println(" V");

  float power12V = vIN * (current12V + current5V + currentON);
  Serial.print("Total Power = ");
  Serial.print(power12V);
  Serial.println(" W");
  softSerial.print("Total Power = ");
  softSerial.print(power12V);
  softSerial.println(" W");

  sensors_event_t humidity, temp;
  
  aht.getEvent(&humidity, &temp);
  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degrees C");
  softSerial.print("Temperature: ");
  softSerial.print(temp.temperature);
  softSerial.println(" degrees C");

  Serial.print("Humidity: ");
  Serial.print(humidity.relative_humidity);
  Serial.println("% rH");
  softSerial.print("Humidity: ");
  softSerial.print(humidity.relative_humidity);
  softSerial.println("% rH");
  //Serial.println();

  if (humidity.relative_humidity > 80) {
    //digitalWrite(fan, HIGH);
    //digitalWrite(relay, HIGH);
  } else {
    //digitalWrite(fan, LOW);
    //digitalWrite(relay, LOW);
  }
  
  chargeState = analogRead(chargeStatePin);
  Serial.print("Battery Status: ");
  if (chargeState > 100) {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.print("Charging || ");
    Serial.println(chargeState);
    softSerial.print("Charging || ");
    softSerial.println(chargeState);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("Used || ");
    Serial.println(chargeState);
    softSerial.print("Used || ");
    softSerial.println(chargeState);
  }

  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  
  Serial.println();


  //Serial.println("|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"); 

  //delay(1000);
}