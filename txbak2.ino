#include <cc1101.h>


  byte i;
  byte version;
  int size = 61;
    byte TX_buffer[61]={0};
    byte RX_buffer[61]={0};
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  TX_buffer[0] = 0x01;
    for(i=1;i<size;i++)
    {
        TX_buffer[i]=i*4;
    }
   Init();
    //_delay_ms(200);
    version = SpiReadStatus(CC1101_VERSION);
    Serial.println("Version");
    Serial.println(version);
}

void loop() {
  // put your main code here, to run repeatedly:
  byte res;
  byte rx_size;
  byte marcstate;

  if(Serial.available() >0){
      byte reading = Serial.read();
      Serial.println("receive");
    if(reading == 49){
      for(;;){
        Serial.println("starting sending the command");
        SpiWriteReg(CC1101_TXFIFO,size);
  //delay(3000);
  SpiWriteBurstReg(CC1101_TXFIFO,TX_buffer,size);      //write data to send
  //delay(3000);
  SpiStrobe(CC1101_STX);                  //start send
  //delay(3000);
  Serial.println("sending marcstate");
  Serial.println(SpiReadStatus(CC1101_MARCSTATE));

 wait_GDO0_high();               // Wait for GDO0 to be set -> sync transmitted
    wait_GDO0_low();

  if((SpiReadStatus(CC1101_TXBYTES) & 0x7F) == 0)
    res = 1;
  SpiStrobe(CC1101_SFTX);

  Serial.println("sent");
  Serial.println(res);
   Serial.println("starting receiving");
   SpiStrobe(CC1101_SIDLE);
   SetReceive();
   marcstate = SpiReadStatus(CC1101_MARCSTATE);
   Serial.println("marcstate is");
  Serial.println(SpiReadStatus(CC1101_MARCSTATE));
  if(marcstate !=1){
    marcstate = SpiReadStatus(CC1101_MARCSTATE);
    _delay_ms(1000);
  }
   rx_size=ReceiveData(RX_buffer);
    Serial.println("size");
    Serial.println(rx_size);
if(rx_size>0){
  for(i=0;i<rx_size;i++)
  {
    Serial.print(RX_buffer[i],DEC);
    Serial.print(" " );
    //Serial.print(' ',BYTE);
  }   
  }
   //_delay_ms(500);
   SpiStrobe(CC1101_SIDLE);
  //Init();
  _delay_ms(500);
  if(Serial.read() == 48)
  break;
      }
       
    }
    else if (reading == 48){
        Serial.println("Waiting for the  command......");
      }
      else{
        Serial.println("Waiting for the  command......");
      }
  }


  
  
 
//  SetReceive();
}
