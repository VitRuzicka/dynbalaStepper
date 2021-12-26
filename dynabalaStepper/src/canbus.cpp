#include "canbus.h"
#include "config.h"


#define bluePillLED PB7

int txMsgID = 0x069; //id zprav, muze slouzit k odfiltrovani nepotrebnych zprav
uint8_t txData[8] = {'n', 'a', 'z', 'd', 'a', 'r', 'r', 'r'}; //uint8_t txData[8]{0x00, 0x01, 0x23, 0x45, 0xab, 0xcd, 0xef, 0xff}; 
uint8_t txDataLen = 8; //8B zprava
uint32_t txDly = 5000; // mSec

//  ****** uncomment the following for the second stm32f103 board ******

// int txMsgID = 0x005;
// uint8_t txData[8]{{0x01, 0xfe, 0xdc, 0xba, 0x11, 0x12, 0x34, 0x56};
// uint_8 txDataLen = 8;
// uint32_t txDly = 1000;  // mSec

int rxMsgid, fltIdx;
uint8_t rxbytes[8];
eXoCAN can;

void CANconfig()
{
  can.begin(STD_ID_LEN, BR500K, PORTA_11_12_WIRE_PULLUP); // 11b IDs, 500k bit rychlost busu, pouzity piny: portA pins 11,12   
  can.filterMask16Init(0, 0, 0x7ff, 0, 0);                // filter bank 0, filter 0: don't pass any, flt 1: pass all msgs
  pinMode(bluePillLED, OUTPUT);
}

uint32_t last = 0;
void CANloop()
{
  /*if (millis() / txDly != last)             // tx every txDly
  {
    last = millis() / txDly;
    can.transmit(txMsgID, txData, txDataLen);
  }*/

  if (can.receive(rxMsgid, fltIdx, rxbytes) > -1){ // poll for rx
   if(rxMsgid == 0x12){
     if(rxbytes[0] == 0x01){ //homovani

     }
     else if(rxbytes[0] == 0x02){ //pohyb

     }
     else if(rxbytes[0] == 0x03){ //nastaveni parametru
     motor_max_spd((int)rxbytes[1]); //maximalni rychlost motoru

     }
     else if(rxbytes[0] == 0x04){ //prikaz k zapnuti/vypnuti motoru 
      toggleMOT();
     }
   }
  else if(rxMsgid == 0x14 && PRAVY){ //pravy motor
  } 
  else if(rxMsgid == 0x16 && !PRAVY){ //levy motor
 
    

  }
  else{
    //nepodporovana zprava
  }
    digitalToggle(bluePillLED); //zmeni stav vzdy kdyz obdrzi zpravu



  
}
}