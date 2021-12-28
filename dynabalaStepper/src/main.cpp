#include <Arduino.h>
#include "canbus.h"
#include <config.h>
#include "motor.h"
//TODO
/*
* nutno pridelat parser na prichazejici prikazy
* implementovat filtr CAN zorav odpovidajici jednotlivemu motoru
* napsat homovaci rutiny pro krokove motory
*/
void setup() {
  
  #ifdef CANBUS
  CANconfig();
  #endif
  motorSetup();

}

void loop() {
  #ifdef CANBUS
  CANloop();
  #endif
  motorLoop();
}