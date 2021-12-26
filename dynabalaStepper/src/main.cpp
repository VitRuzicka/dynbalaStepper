#include <Arduino.h>
#include "canbus.h"
#include <SimpleFOC.h>
#include <config.h>
//TODO
/*
* nutno pridelat parser na prichazejici prikazy
* implementovat filtr CAN zorav odpovidajici jednotlivemu motoru
* napsat homovaci rutiny pro krokove motory
*/
bool stavMOT = false;
MagneticSensorI2C sensor = MagneticSensorI2C(0x36, 12, 0x0E, 4);  //0x36 je i2c adresa toho enkoderu, 12 je bit resolution, 0x0E je adresa registru s daty, 4 pocet bitu v MSB registru
StepperDriver4PWM driver = StepperDriver4PWM(PA6, PA7, PB0, PB1); //civka 1 , civka 1, civka 2, civka 2 mozna bude treba predelat aby to chodilo spravnym smerem
StepperMotor motor = StepperMotor(50); // 200kroku na otacku/4


void motor_max_spd(uint8_t rychlost = FEEDRATE){
  motor.velocity_limit = rychlost; //rad/s
}
void toggleMOTOR(){
stavMOT ? motor.disable() : motor.enable();
}
void setup() {
  pinMode(ENDSTOP, INPUT);
  #ifdef CANBUS
  CANconfig();
  #endif/*
  #ifdef CLOSEDLOOP
  sensor.init(); //senzor na krokáči
  #endif
  driver.voltage_power_supply = 24;
  driver.init();
  #ifdef CLOSEDLOOP
  motor.linkSensor(&sensor); //netreba v otevrene smycce
  #endif
  motor.linkDriver(&driver);
  // MotionControlType::torque      - torque control loop using voltage
  // MotionControlType::velocity    - velocity motion control
  // MotionControlType::angle       - position/angle motion control
  motor.controller = MotionControlType::angle; //mozna bude potreba torque mode

  motor.init();
  #ifdef CLOSEDLOOP
  motor.initFOC();
  #endif*/
  stavMOT = true;
}

void loop() {
  #ifdef CANBUS
  CANloop();
  #endif/*
  #ifdef CLOSEDLOOP
  motor.loopFOC();
  #endif
  //pohyb pomoci:
  //motor.move(uhel) //float
  //motor.sensor_offset = motor.shaft_angle; //v podstate nulovani hodnoty enkoderu - pouzito u homovani
  */
}