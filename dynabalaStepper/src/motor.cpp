#include <motor.h>
#include <SimpleFOC.h>
#include "config.h"

bool aktivniMOT = false;
bool homovano = false;
unsigned long casSpusteni = 0;

MagneticSensorI2C sensor = MagneticSensorI2C(AS5600_I2C);  //0x36 je i2c adresa toho enkoderu, 12 je bit resolution, 0x0E je adresa registru s daty, 4 pocet bitu v MSB registru
#ifdef PRAVY
StepperDriver4PWM driver = StepperDriver4PWM(PA6, PA7, PB0, PB1); //civka 1 , civka 1, civka 2, civka 2 mozna bude treba predelat aby to chodilo spravnym smerem
#else
StepperDriver4PWM driver = StepperDriver4PWM(PB1, PB0, PA7, PA6); //civka 1 , civka 1, civka 2, civka 2 mozna bude treba predelat aby to chodilo spravnym smerem
#endif
StepperMotor motor = StepperMotor(50); // 200kroku na otacku/4


void motor_max_spd(uint8_t rychlost = FEEDRATE){
  motor.velocity_limit = rychlost; //rad/s
}
void toggleMot(){
    homovano = false;
    aktivniMOT ? motor.disable() : motor.enable();
}
void motorSetup(){
  pinMode(ENDSTOP, INPUT);
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
  motor.controller = MotionControlType::angle; //velocity pouzito pro homing

  motor.init();
  #ifdef CLOSEDLOOP
  motor.initFOC();
  #endif
  aktivniMOT = true;
}
void motorLoop(){
  #ifdef CLOSEDLOOP
  motor.loopFOC();
  #endif
  
 }
void tocMotorem(float uhel){ //nastavi urcitou hodnotu na uhel otaceni
    motor.move(SMER_OTACENI*uhel*POMER_PREVODOVKA); //float
}
void nulujPolohu(){
    motor.sensor_offset = motor.shaft_angle; //v podstate nulovani hodnoty enkoderu - pouzito u homovani

}
void homujMotor(){
   // if(!homovano){
        casSpusteni = millis(); 
        motor.controller = MotionControlType::velocity; //mozna bude potreba torque mode

        while((digitalRead(ENDSTOP) != AKTIVNI_ENDSTOP )&& ((millis() - casSpusteni)  < TIMEOUT_HOMOVANI) ){ //toc motorem dokud nenajede na koncak nebo nedojde cas
            tocMotorem(-1 * SMER_OTACENI * FEEDRATE_HOMOVANI);
            motorLoop();
        }
        tocMotorem(0); //zastav motor
        motorLoop();
        motor.controller = MotionControlType::angle; //mozna bude potreba torque mode
        nulujPolohu();
        tocMotorem(ODJETI_Z_KONCAKU);
        homovano = true;
    //}
  
}