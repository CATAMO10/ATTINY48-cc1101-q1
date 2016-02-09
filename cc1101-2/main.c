#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "TWI_Master.h"
#include "cc1101.h"
#define slaveaddress    0x04
#define size 61

byte TX_buffer[size]={0};
byte i;

int main(void) {
    byte version;
    for(i=0;i<size;i++)
    {
        TX_buffer[i]=i*2;
    }
    Init();
    version = SpiReadStatus(CC1101_VERSION);
    if (!TWIM_Init (100000))
    {
        
        while (1);
    }
    /*
     ** Endless loop
     */
    while (1)
    {
        byte sent;
        //sent = SendData(TX_buffer,size);
        byte marcstate,marcstate2;
        SpiWriteReg(CC1101_TXFIFO,size);
        SpiWriteBurstReg(CC1101_TXFIFO,TX_buffer,size);      //write data to send
        SpiStrobe(CC1101_STX);                  //start send
        marcstate = SpiReadStatus(CC1101_MARCSTATE);
        wait_GDO0_high();
        wait_GDO0_low();
        marcstate2 = SpiReadStatus(CC1101_MARCSTATE);
//        if((SpiReadStatus(CC1101_TXBYTES) & 0x7F) == 0)
//            res = 1;
        
        SpiStrobe(CC1101_SFTX);
        if (!TWIM_Start (slaveaddress, TWIM_WRITE))
        {
            TWIM_Stop ();
        }
        else
        {
            TWIM_Write(version);
            TWIM_Write(marcstate);
            TWIM_Write(marcstate2);
            TWIM_Stop ();
            _delay_ms (300);
        }
        
    }
}

