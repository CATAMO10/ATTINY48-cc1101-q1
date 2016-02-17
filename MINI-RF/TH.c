//#include <stdio.h>
//#include <avr/io.h>
//#include <avr/interrupt.h>
//#include <util/delay.h>
//#include <stdbool.h>
//#include "TWI_Master.h"
//#include "cc1101.h"
//#include "pins_arduino.h"
//#define slaveaddress    0x04
//#define slaveAddress2   0x40
//#define slaveAddress3   0x68
//#define bit(b) (1UL << (b))
//#define tx_size 61
//
//byte TX_buffer[tx_size]={0};
//byte i;
//
//byte RX_buffer[61]={0};
//byte i;
//byte available=false;
//void pciSetup(byte pin);
//ISR (PCINT2_vect);
//int main(void) {
//    byte version;
//    byte size = 0x00;
//    byte marcstate;
//    byte res;
//    uint32_t X0,X1,Y0,Y1,Y2;
//    uint32_t X_out,Y_out1,Y_out2;
//    
//    Init();
//    version = SpiReadStatus(CC1101_VERSION);
//    SetReceive();
//    TX_buffer[0] = 0x01;
//    for(i=1;i<tx_size;i++)
//    {
//        TX_buffer[i]=i*10;
//    }
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
//        uint32_t X=0, Y=0;
//        uint16_t AcX = 0;
//        if(!TWIM_Start(slaveAddress2, TWIM_WRITE))
//        {
//            TWIM_Stop();
//            
//        }
//        else{
//            TWIM_Write(0xE3);
//            TWIM_Stop();
//        }
//        if (!TWIM_Start (slaveAddress2, TWIM_READ))
//        {
//            TWIM_Stop ();
//        }
//        else
//        {
//            X0=TWIM_ReadAck();
//            X1=TWIM_ReadNack();
//            X0=X0<<8;
//            X_out=X0+X1;
//            X=(175.72*X_out)/65536;
//            X=X-46.85;
//            TX_buffer[1] = X;
//        }
//        if(!TWIM_Start(slaveAddress2, TWIM_WRITE))
//        {
//            TWIM_Stop();
//            
//        }
//        else{
//            TWIM_Write(0xE5);
//            TWIM_Stop();
//        }
//        if (!TWIM_Start (slaveAddress2, TWIM_READ))
//        {
//            TWIM_Stop ();
//        }
//        else
//        {
//            Y0=TWIM_ReadAck();
//            Y1=TWIM_ReadNack();
//            Y2=Y0/100;
//            Y0=Y0%100;
//            Y_out1 = Y2*25600;
//            Y_out2 = Y0*256+Y1;
//            Y_out1 = (125*Y_out1)/65536;
//            Y_out2 = (125*Y_out2)/65536;
//            Y = Y_out1+Y_out2;
//            Y=Y-6;
//            TX_buffer[2] = Y;
//            
//            
//            marcstate = SpiReadStatus(CC1101_MARCSTATE);
//            if(SpiReadStatus(CC1101_MARCSTATE) != 1){
//                marcstate = SpiReadStatus(CC1101_MARCSTATE);
//                _delay_ms(1000);
//            }
//            size=ReceiveData(RX_buffer);
//            
//            if (size>0) {
//                if(RX_buffer[0] == 0x01){
//                    SpiWriteReg(CC1101_TXFIFO,tx_size);
//                    SpiWriteBurstReg(CC1101_TXFIFO,TX_buffer,tx_size);      //write data to send
//                    SpiStrobe(CC1101_STX);                  //start send
//                    wait_GDO0_high();
//                    wait_GDO0_low();
//                    if((SpiReadStatus(CC1101_TXBYTES) & 0x7F) == 0)
//                        res = 1;
//                    SpiStrobe(CC1101_SFTX);
//                }
//            }
//        }
//        _delay_ms(1500);
//        SetReceive();
//    }
//}
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
