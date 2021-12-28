#include "canbus.h"
#include "config.h"
#include "motor.h"

#define BUILTIN_LED PB7

int pingMsgId = 0x20; //id zprav, muze slouzit k odfiltrovani nepotrebnych zprav
uint8_t pingData[8] = {'p', 'i', 'n', 'g', 0x00, 0x00, 0x00, 0x00}; //uint8_t pingData[8]{0x00, 0x01, 0x23, 0x45, 0xab, 0xcd, 0xef, 0xff}; 
uint8_t pingDataLen = 8; //8B zprava
uint32_t txDly = 5000; // mSec

int rxMsgid, fltIdx;
uint8_t rxbytes[8];
eXoCAN can;

void CANconfig()
{
  can.begin(STD_ID_LEN, BR500K, PORTA_11_12_WIRE_PULLUP); // 11b IDs, 500k bit rychlost busu, pouzity piny: portA pins 11,12   
  can.filterMask16Init(0, 0, 0x7ff, 0, 0);                // filter bank 0, filter 0: don't pass any, flt 1: pass all msgs
  pinMode(BUILTIN_LED, OUTPUT);
}

uint32_t last = 0;
void CANloop()
{
  /*if (millis() / txDly != last)             // na test CAN komunikace
  {
    last = millis() / txDly;
    can.transmit(pingMsgId, pingData, pingDataLen);
  }*/

  if (can.receive(rxMsgid, fltIdx, rxbytes) > -1){ // poll for rx
   //POHYB
   if(  (rxMsgid == 0x12)  || (rxMsgid == 0x14 && PRAVY) ||  (rxMsgid == 0x16 && !PRAVY) ){
     if(rxbytes[0] == 0x01){ //homovani
      homujMotor();
     }
     else if(rxbytes[0] == 0x02){ //pohyb
      float uhel = (int)rxbytes[2] + map((int)rxbytes[3], 0, 255, 0.01, 0.99) ; //treti index (2) udava cele stupne, ctvrty (3) udava desetiny
      if(uhel < LIMIT_POHYBU ){
        if(rxbytes[1] == 0x02){ //obraceni smeru pohybu
            uhel = uhel * -1; 
        }
           tocMotorem(uhel);
      }
     
     }
     else if(rxbytes[0] == 0x03){ //nastaveni parametru
      motor_max_spd((int)rxbytes[1]); //maximalni rychlost motoru

     }
     else if(rxbytes[0] == 0x04){ //prikaz k zapnuti/vypnuti motoru 
      toggleMot();
     }
   }
   //PING:
   else if(rxMsgid == 0x18){
     can.transmit(pingMsgId, pingData, pingDataLen); //odeslani PONGu
   }
  //COKOLIV JINEHO
  else{
    //nepodporovana zprava
  }
    digitalToggle(BUILTIN_LED); //zmeni stav vzdy kdyz obdrzi zpravu



  
}
}