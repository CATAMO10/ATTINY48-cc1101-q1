#include <cc1101.h>


/****************************************************************/
#define     WRITE_BURST         0x40                        //write burst
#define     READ_SINGLE         0x80                        //read single
#define     READ_BURST          0xC0                        //read burst
#define     BYTES_IN_RXFIFO     0x7F                        //byte number in RXfifo

/****************************************************************/
byte PaTabel[8] = {0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60};

/****************************************************************
 *FUNCTION NAME:SpiInit
 *FUNCTION     :spi communication initialization
 *INPUT        :none
 *OUTPUT       :none
 ****************************************************************/
void SpiInit(void)
{
    
    //digitalWrite(SS_PIN, HIGH);
    PORTB = (1<<PB2);
    // Configure SPI pins
    DDRB = (1<<DDB3) | (1<<DDB5) | (1<<DDB2);
    // pinMode(SS_PIN, OUTPUT);
    // pinMode(MOSI_PIN, OUTPUT);
    // pinMode(MISO_PIN, INPUT);
    // pinMode(SCK_PIN, OUTPUT);
    PORTB = (1<<PB5);
    PORTB &= ~(1<<PB4);
    // digitalWrite(SCK_PIN, HIGH);
    // digitalWrite(MOSI_PIN, LOW);
    
    // SPI speed = clk/4
    SPCR = _BV(SPE) | _BV(MSTR);
    // SPCR = ~_BV(SPR0) | ~_BV(SPI2X) | ~_BV(SPR1);
}
/****************************************************************
 *FUNCTION NAME:SpiTransfer
 *FUNCTION     :spi transfer
 *INPUT        :value: data to send
 *OUTPUT       :data to receive
 ****************************************************************/
byte SpiTransfer(byte value)
{
    SPDR = value;
    while (!(SPSR & (1<<SPIF))) ;
    return SPDR;
}

/****************************************************************
 *FUNCTION NAME: GDO_Set()
 *FUNCTION     : set GDO0,GDO2 pin
 *INPUT        : none
 *OUTPUT       : none
 ****************************************************************/
void GDO_Set (void)
{
    // pinMode(GDO0, INPUT);
    // pinMode(GDO2, INPUT);
    DDRD &= ~(1<<DDD2);
}

/****************************************************************
 *FUNCTION NAME:Reset
 *FUNCTION     :CC1101 reset //details refer datasheet of CC1101/CC1100//
 *INPUT        :none
 *OUTPUT       :none
 ****************************************************************/
void Reset (void)
{
    //digitalWrite(SS_PIN, LOW);
    PORTB &= ~(1<<PB2);
    _delay_ms(1);
    //digitalWrite(SS_PIN, HIGH);
    PORTB |= (1<<PB2);
    _delay_ms(1);
    //digitalWrite(SS_PIN, LOW);
    PORTB &= ~(1<<PB2);
    //while(digitalRead(MISO_PIN));
    wait_Miso();
    SpiTransfer(CC1101_SRES);
    //while(digitalRead(MISO_PIN));
    wait_Miso();
    //digitalWrite(SS_PIN, HIGH);]
    PORTB = (1<<PB2);
}

/****************************************************************
 *FUNCTION NAME:Init
 *FUNCTION     :CC1101 initialization
 *INPUT        :none
 *OUTPUT       :none
 ****************************************************************/
void Init(void)
{
    SpiInit();                                      //spi initialization
    GDO_Set();                                     //GDO set
    //digitalWrite(SS_PIN, HIGH);
    PORTB = (1<<PB2);
    //digitalWrite(SCK_PIN, HIGH);
    PORTB = (1<<PB5);
    //digitalWrite(MOSI_PIN, LOW);
    PORTB &= ~(1<<PB3);
    Reset();                                       //CC1101 reset
    RegConfigSettings();                           //CC1101 register config
    SpiWriteBurstReg(CC1101_PATABLE,PaTabel,8);        //CC1101 PATABLE config
}


/****************************************************************
 *FUNCTION NAME:SpiWriteReg
 *FUNCTION     :CC1101 write data to register
 *INPUT        :addr: register address; value: register value
 *OUTPUT       :none
 ****************************************************************/
void SpiWriteReg(byte addr, byte value)
{
    //digitalWrite(SS_PIN, LOW);
    PORTB &= ~(1<<PB2);
    //while(digitalRead(MISO_PIN));
    wait_Miso();
    SpiTransfer(addr);
    SpiTransfer(value);
    //digitalWrite(SS_PIN, HIGH);
}

/****************************************************************
 *FUNCTION NAME:SpiWriteBurstReg
 *FUNCTION     :CC1101 write burst data to register
 *INPUT        :addr: register address; buffer:register value array; num:number to write
 *OUTPUT       :none
 ****************************************************************/
void SpiWriteBurstReg(byte addr, byte *buffer, byte num)
{
    byte i, temp;
    
    temp = addr | WRITE_BURST;
    //digitalWrite(SS_PIN, LOW);
    PORTB &= ~(1<<PB2);
    //while(digitalRead(MISO_PIN));
    wait_Miso();
    SpiTransfer(temp);
    for (i = 0; i < num; i++)
    {
        SpiTransfer(buffer[i]);
    }
    //digitalWrite(SS_PIN, HIGH);
    PORTB = (1<<PB2);
}

/****************************************************************
 *FUNCTION NAME:SpiStrobe
 *FUNCTION     :CC1101 Strobe
 *INPUT        :strobe: command; //refer define in CC1101.h//
 *OUTPUT       :none
 ****************************************************************/
void SpiStrobe(byte strobe)
{
    //digitalWrite(SS_PIN, LOW);
    PORTB &= ~(1<<PB2);
    //while(digitalRead(MISO_PIN));
    wait_Miso();
    SpiTransfer(strobe);
    //digitalWrite(SS_PIN, HIGH);
    PORTB = (1<<PB2);
}

/****************************************************************
 *FUNCTION NAME:SpiReadReg
 *FUNCTION     :CC1101 read data from register
 *INPUT        :addr: register address
 *OUTPUT       :register value
 ****************************************************************/
byte SpiReadReg(byte addr)
{
    byte temp, value;
    
    temp = addr|READ_SINGLE;
    //digitalWrite(SS_PIN, LOW);
    PORTB &= ~(1<<PB2);
    //while(digitalRead(MISO_PIN));
    wait_Miso();
    SpiTransfer(temp);
    value=SpiTransfer(0);
    //digitalWrite(SS_PIN, HIGH);
    PORTB = (1<<PB2);
    return value;
}

/****************************************************************
 *FUNCTION NAME:SpiReadBurstReg
 *FUNCTION     :CC1101 read burst data from register
 *INPUT        :addr: register address; buffer:array to store register value; num: number to read
 *OUTPUT       :none
 ****************************************************************/
void SpiReadBurstReg(byte addr, byte *buffer, byte num)
{
    byte i,temp;
    
    temp = addr | READ_BURST;
    //digitalWrite(SS_PIN, LOW);
    PORTB &= ~(1<<PB2);
    //while(digitalRead(MISO_PIN));
    wait_Miso();
    SpiTransfer(temp);
    for(i=0;i<num;i++)
    {
        buffer[i]=SpiTransfer(0);
    }
    //digitalWrite(SS_PIN, HIGH);
    PORTB = (1<<PB2);
}

/****************************************************************
 *FUNCTION NAME:SpiReadStatus
 *FUNCTION     :CC1101 read status register
 *INPUT        :addr: register address
 *OUTPUT       :status value
 ****************************************************************/
byte SpiReadStatus(byte addr)
{
    byte value,temp;
    
    temp = addr | READ_BURST;
    //digitalWrite(SS_PIN, LOW);
    PORTB &= ~(1<<PB2);
    //while(digitalRead(MISO_PIN));
    wait_Miso();
    SpiTransfer(temp);
    value=SpiTransfer(0);
    //digitalWrite(SS_PIN, HIGH);
    PORTB = (1<<PB2);
    return value;
}

/****************************************************************
 *FUNCTION NAME:RegConfigSettings
 *FUNCTION     :CC1101 register config //details refer datasheet of CC1101/CC1100//
 *INPUT        :none
 *OUTPUT       :none
 ****************************************************************/
void RegConfigSettings(void)
{
    SpiWriteReg(CC1101_FSCTRL1,  0x08);
    SpiWriteReg(CC1101_FSCTRL0,  0x00);
    SpiWriteReg(CC1101_FREQ2,    0x10);
    SpiWriteReg(CC1101_FREQ1,    0xA7);
    SpiWriteReg(CC1101_FREQ0,    0x62);
    SpiWriteReg(CC1101_MDMCFG4,  0x5B);
    SpiWriteReg(CC1101_MDMCFG3,  0xF8);
    SpiWriteReg(CC1101_MDMCFG2,  0x03);
    SpiWriteReg(CC1101_MDMCFG1,  0x22);
    SpiWriteReg(CC1101_MDMCFG0,  0xF8);
    SpiWriteReg(CC1101_CHANNR,   0x00);
    SpiWriteReg(CC1101_DEVIATN,  0x47);
    SpiWriteReg(CC1101_FREND1,   0xB6);
    SpiWriteReg(CC1101_FREND0,   0x10);
    SpiWriteReg(CC1101_MCSM0 ,   0x18);
    SpiWriteReg(CC1101_FOCCFG,   0x1D);
    SpiWriteReg(CC1101_BSCFG,    0x1C);
    SpiWriteReg(CC1101_AGCCTRL2, 0xC7);
    SpiWriteReg(CC1101_AGCCTRL1, 0x00);
    SpiWriteReg(CC1101_AGCCTRL0, 0xB2);
    SpiWriteReg(CC1101_FSCAL3,   0xEA);
    SpiWriteReg(CC1101_FSCAL2,   0x2A);
    SpiWriteReg(CC1101_FSCAL1,   0x00);
    SpiWriteReg(CC1101_FSCAL0,   0x11);
    SpiWriteReg(CC1101_FSTEST,   0x59);
    SpiWriteReg(CC1101_TEST2,    0x81);
    SpiWriteReg(CC1101_TEST1,    0x35);
    SpiWriteReg(CC1101_TEST0,    0x09);
    SpiWriteReg(CC1101_IOCFG2,   0x0B);     //serial clock.synchronous to the data in synchronous serial mode
    SpiWriteReg(CC1101_IOCFG0,   0x06);     //asserts when sync word has been sent/received, and de-asserts at the end of the packet
    SpiWriteReg(CC1101_PKTCTRL1, 0x04);     //two status bytes will be appended to the payload of the packet,including RSSI LQI and CRC OK
    //No address check
    SpiWriteReg(CC1101_PKTCTRL0, 0x05);     //whitening off;CRC Enable£»variable length packets, packet length configured by the first byte after sync word
    SpiWriteReg(CC1101_ADDR,     0x00);     //address used for packet filtration.
    SpiWriteReg(CC1101_PKTLEN,   0x3D);     //61 bytes max length
}

/****************************************************************
 *FUNCTION NAME:SendData
 *FUNCTION     :use CC1101 send data
 *INPUT        :txBuffer: data array to send; size: number of data to send, no more than 61
 *OUTPUT       :none
 ****************************************************************/
byte SendData(byte *txBuffer,byte size)
{
    byte res = 0x00;
    SpiWriteReg(CC1101_TXFIFO,size);
    SpiWriteBurstReg(CC1101_TXFIFO,txBuffer,size);          //write data to send
    SpiStrobe(CC1101_STX);                                  //start send
    //while (!digitalRead(GDO0));                             // Wait for GDO0 to be set -> sync transmitted
    //while (digitalRead(GDO0));                              // Wait for GDO0 to be cleared -> end of packet
    wait_GDO0_high();
    wait_GDO0_low();
    if((SpiReadStatus(CC1101_TXBYTES) & 0x7F) == 0)
        res = 1;
    
    
    SpiStrobe(CC1101_SFTX);                                 //flush TXfifo
    return res;
    
}

/****************************************************************
 *FUNCTION NAME:SetReceive
 *FUNCTION     :set CC1101 to receive state
 *INPUT        :none
 *OUTPUT       :none
 ****************************************************************/
void SetReceive(void)
{
    SpiStrobe(CC1101_SRX);
}

/****************************************************************
 *FUNCTION NAME:CheckReceiveFlag
 *FUNCTION     :check receive data or not
 *INPUT        :none
 *OUTPUT       :flag: 0 no data; 1 receive data
 ****************************************************************/
// byte CheckReceiveFlag(void)
// {
//     if(digitalRead(GDO0))           //receive data
//     {
//         while (digitalRead(GDO0));
//         return 1;
//     }
//     else                            // no data
//     {
//         return 0;
//     }
// }


/****************************************************************
 *FUNCTION NAME:ReceiveData
 *FUNCTION     :read data received from RXfifo
 *INPUT        :rxBuffer: buffer to store data
 *OUTPUT       :size of data received
 ****************************************************************/
byte ReceiveData(byte *rxBuffer)
{
    byte size;
    byte status[2];
    
    if(SpiReadStatus(CC1101_RXBYTES) & BYTES_IN_RXFIFO)
    {
        size=SpiReadReg(CC1101_RXFIFO);
        SpiReadBurstReg(CC1101_RXFIFO,rxBuffer,size);
        SpiReadBurstReg(CC1101_RXFIFO,status,2);
        SpiStrobe(CC1101_SFRX);
        return size;
    }
    else
    {
        SpiStrobe(CC1101_SFRX);
        return 0;
    }
    
}