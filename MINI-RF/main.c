#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include "TWI_Master.h"
#include "cc1101.h"
#include "pins_arduino.h"
//#include "ext_interrupt.h"
#define slaveaddress    0x04
#define bit(b) (1UL << (b))
//#define size 61

byte RX_buffer[61]={0};
byte i;
byte available=false;
void pciSetup(byte pin);
ISR (PCINT2_vect);
int main(void) {
    byte version;
    byte size = 0x00;
    byte marcstate;
    Init();
    version = SpiReadStatus(CC1101_VERSION);
    SetReceive();
        if (!TWIM_Init (100000))
        {
    
            while (1);
        }
    /*
     ** Endless loop
     */
    while (1)
    {
        
        if (!TWIM_Start (slaveaddress, TWIM_WRITE))
        {
            TWIM_Stop ();
        }
        else
        {
        //sent = SendData(TX_buffer,size);
            //byte res;
            TWIM_Write(version);
            marcstate = SpiReadStatus(CC1101_MARCSTATE);
            if(SpiReadStatus(CC1101_MARCSTATE) != 1){
                marcstate = SpiReadStatus(CC1101_MARCSTATE);
                _delay_ms(1000);
            }
            size=ReceiveData(RX_buffer);
            
            if (size>0) {
                TWIM_Write(RX_buffer[8]);
            }
            TWIM_Write(size);
            TWIM_Stop ();
        }
        
        _delay_ms(4000);
        SetReceive();
        //pciSetup(5);
    }
}
void pciSetup(byte pin)
{
    *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
    PCIFR  |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
    PCICR  |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
}
ISR (PCINT2_vect) // handle pin change interrupt for D0 to D7 here
{
    available = true;
}

//
//
//
//
//
//#include <stdio.h>
//#include <avr/io.h>
//#include <avr/interrupt.h>
//#include <util/delay.h>
//#include "TWI_Master.h"
//#include "cc1101.h"
//#define slaveaddress    0x04
//#define size 61
//
//byte TX_buffer[size]={0};
//byte i;
//
//int main(void) {
//    byte version;
//    for(i=0;i<size;i++)
//    {
//        TX_buffer[i]=i*10;
//    }
//    Init();
//    //_delay_ms(200);
//    version = SpiReadStatus(CC1101_VERSION);
//    if (!TWIM_Init (100000))
//    {
//
//        while (1);
//    }
//    /*
//     ** Endless loop
//     */
//    while (1)
//    {
//
//        if (!TWIM_Start (slaveaddress, TWIM_WRITE))
//        {
//            TWIM_Stop ();
//        }
//        else
//        {
//            //sent = SendData(TX_buffer,size);
//            byte res;
//            TWIM_Write(version);
//            SpiWriteReg(CC1101_TXFIFO,size);
//            //_delay_ms(1000);
//            SpiWriteBurstReg(CC1101_TXFIFO,TX_buffer,size);      //write data to send
//            //_delay_ms(1000);
//            SpiStrobe(CC1101_STX);                  //start send
//            //_delay_ms(1000);
//            TWIM_Write(SpiReadStatus(CC1101_MARCSTATE));
//            wait_GDO0_high();
//            wait_GDO0_low();
//            //TWIM_Write(SpiReadStatus(CC1101_MARCSTATE));
//            if((SpiReadStatus(CC1101_TXBYTES) & 0x7F) == 0)
//                res = 1;
//
//            TWIM_Write(res);
//            SpiStrobe(CC1101_SFTX);
//            //_delay_ms(1000);
//
//            TWIM_Stop ();
//        }
//
//        _delay_ms(2000);
//
//    }
//}





















