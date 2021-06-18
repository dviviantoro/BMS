#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219_Bat;
Adafruit_INA219 ina219_Dis(0x41);
Adafruit_INA219 ina219_Rasp(0x44);
Adafruit_INA219 ina219_5V(0x45);

const int relay = 10;
const int limitSwitch = 11;
int switchState = 0;


void setup() {
  Serial.begin(115200);
  ina219_Bat.begin();
  ina219_Dis.begin();
  ina219_Rasp.begin();
  ina219_5V.begin();

  pinMode(limitSwitch, INPUT);
  pinMode(relay, OUTPUT);

  // By default the initialization will use the largest range (32V, 2A).  
  
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  //ina219.setCalibration_32V_1A();
  
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  ina219_Bat.setCalibration_16V_400mA();
}

void loop() {
  switchState = digitalRead(limitSwitch);
  if (switchState == HIGH) {
    digitalWrite(relay, HIGH);
  } else {
    digitalWrite(relay, LOW);
  }

  float shuntvoltageBat = 0;
  float busvoltageBat = 0;
  float currentBat = 0;
  float loadvoltageBat = 0;
  float powerBat = 0;

  float shuntvoltageDis = 0;
  float busvoltageDis = 0;
  float currentDis = 0;
  float loadvoltageDis = 0;
  float powerDis = 0;

  float shuntvoltageRasp = 0;
  float busvoltageRasp = 0;
  float currentRasp = 0;
  float loadvoltageRasp = 0;
  float powerRasp = 0;

  float shuntvoltage5V = 0;
  float busvoltage5V = 0;
  float current5V = 0;
  float loadvoltage5V = 0;
  float power5V = 0;

  shuntvoltageBat = ina219_Bat.getShuntVoltage_mV();
  busvoltageBat = ina219_Bat.getBusVoltage_V();
  currentBat = ina219_Bat.getCurrent_mA();
  powerBat = ina219_Bat.getPower_mW();
  loadvoltageBat = busvoltageBat + (shuntvoltageBat / 1000);

  shuntvoltageDis = ina219_Dis.getShuntVoltage_mV();
  busvoltageDis = ina219_Dis.getBusVoltage_V();
  currentDis = ina219_Dis.getCurrent_mA();
  powerDis = ina219_Dis.getPower_mW();
  loadvoltageDis = busvoltageDis + (shuntvoltageDis / 1000);

  shuntvoltageRasp = ina219_Rasp.getShuntVoltage_mV();
  busvoltageRasp = ina219_Rasp.getBusVoltage_V();
  currentRasp = ina219_Rasp.getCurrent_mA();
  powerRasp = ina219_Rasp.getPower_mW();
  loadvoltageRasp = busvoltageRasp + (shuntvoltageRasp / 1000);

  shuntvoltage5V = ina219_5V.getShuntVoltage_mV();
  busvoltage5V = ina219_5V.getBusVoltage_V();
  current5V = ina219_5V.getCurrent_mA();
  power5V = ina219_5V.getPower_mW();
  loadvoltage5V = busvoltage5V + (shuntvoltage5V / 1000);

  Serial.print("Voltage battery: ");
  Serial.print(loadvoltageBat);
  Serial.println(" V");
  Serial.print("Current battery: ");
  Serial.print(currentBat);
  Serial.println(" mA");
  Serial.print("Power battery: ");
  Serial.print(powerBat);
  Serial.println(" mW");

  Serial.print("Voltage display: ");
  Serial.print(loadvoltageDis);
  Serial.println(" V");
  Serial.print("Current display: ");
  Serial.print(currentDis);
  Serial.println(" mA");
  Serial.print("Power display: ");
  Serial.print(powerDis);
  Serial.println(" mW");

  Serial.print("Voltage raspberry: ");
  Serial.print(loadvoltageRasp);
  Serial.println(" V");
  Serial.print("Current raspberry: ");
  Serial.print(currentRasp);
  Serial.println(" mA");
  Serial.print("Power display: ");
  Serial.print(powerRasp);
  Serial.println(" mW");

  Serial.print("Voltage 5V bus: ");
  Serial.print(loadvoltage5V);
  Serial.println(" V");
  Serial.print("Current 5V bus: ");
  Serial.print(current5V);
  Serial.println(" mA");
  Serial.print("Power 5V bus: ");
  Serial.print(power5V);
  Serial.println(" mW");

  Serial.println();
  Serial.println();

  //delay(2000);
}