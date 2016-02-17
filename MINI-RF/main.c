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
#define slaveAddress2   0x40
#define slaveAddress3   0x68
#define slaveAddress4   (0x48>>1)
#define bit(b) (1UL << (b))
#define tx_size 61

byte TX_buffer[tx_size]={0};
byte i;

byte RX_buffer[61]={0};
byte i;
byte available=false;
void pciSetup(byte pin);
ISR (PCINT2_vect);
int main(void) {
    byte version;
    byte size = 0x00;
    byte marcstate;
    byte res;
    uint32_t X0,X1,Y0,Y1,Y2;
    uint32_t X_out,Y_out1,Y_out2;
    
    Init();
    version = SpiReadStatus(CC1101_VERSION);
    SetReceive();
    TX_buffer[0] = 0x02;//Accerometer
    for(i=1;i<tx_size;i++)
            {
                TX_buffer[i]=i*10;
            }
        if (!TWIM_Init (100000))
        {
    
            while (1);
        }
    /*
     ** Endless loop
     */
    while (1)
    {
        uint32_t X=0, Y=0,respond;
        uint16_t AcX = 0;
        uint8_t MSBH,MSBL,LSBH,LSBL;
        DDRD = (1<<6);
        if(!TWIM_Start(slaveAddress4, TWIM_WRITE))
        {
            TWIM_Stop();
            
        }
        else{
            TWIM_Write(0x00);
            TWIM_Write(0x00);
            TWIM_Write(0xFF);
            TWIM_Write(0x02);
            TWIM_Write(0xFE);
            
            TWIM_Write(0xD4);
            TWIM_Write(0x02);//get firmware
            TWIM_Write(0x2A);
            TWIM_Write(0x00);
            PORTD = (1<<6);
            TWIM_Stop();
        }
        if (!TWIM_Start (slaveAddress4, TWIM_READ))
        {
            TWIM_Stop ();
        }
        else
        {
//            TWIM_ReadAck();
//            TWIM_ReadAck();
//            TWIM_ReadAck();
//            TWIM_ReadAck();
//            TWIM_ReadAck();
            
            MSBH = TWIM_ReadAck();
            respond = MSBH;
            respond <<=8;
            MSBL = TWIM_ReadAck();
            respond|=MSBL;
            respond <<=8;
            LSBH = TWIM_ReadAck();
            respond |=LSBH;
            respond <<=8;
            LSBL = TWIM_ReadAck();
            respond |=LSBL;
            //TX_buffer[1] = MSB;
            //TX_buffer[2] = LSB;
            PORTD &= ~(1<<6);
            TWIM_Stop();
        }

        if (!TWIM_Start (slaveaddress, TWIM_WRITE))
        {
            TWIM_Stop ();
        }
        else
        {
            marcstate = SpiReadStatus(CC1101_MARCSTATE);
            if(SpiReadStatus(CC1101_MARCSTATE) != 1){
                marcstate = SpiReadStatus(CC1101_MARCSTATE);
                _delay_ms(1000);
            }
            size=ReceiveData(RX_buffer);
            TWIM_Write(MSBH);
            TWIM_Write(MSBL);
            TWIM_Write(LSBH);
            TWIM_Write(LSBL);
            
            if (size>0) {
                TWIM_Write(RX_buffer[8]);
                
                if(RX_buffer[0] == 0x02){
                    SpiWriteReg(CC1101_TXFIFO,tx_size);
                        SpiWriteBurstReg(CC1101_TXFIFO,TX_buffer,tx_size);      //write data to send
                        SpiStrobe(CC1101_STX);                  //start send
                        //TWIM_Write(SpiReadStatus(CC1101_MARCSTATE));
                        wait_GDO0_high();
                        wait_GDO0_low();
                        if((SpiReadStatus(CC1101_TXBYTES) & 0x7F) == 0)
                            res = 1;
            
                        //TWIM_Write(res);
                    //PORTD = (1<<6);
                        SpiStrobe(CC1101_SFTX);
                }
            }
            //TWIM_Write(size);
           TWIM_Stop ();
       }
        _delay_ms(1500);
        SetReceive();
        PORTD &= ~(1<<6);
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





















