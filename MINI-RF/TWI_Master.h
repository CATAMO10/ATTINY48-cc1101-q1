/*******************************************************
 Author:					Manfred Langemann
 mailto:					Manfred.Langemann ät t-online.de
 Begin of project:			04.01.2008
 Latest version generated:	04.01.2008
 Filename:					TWI_Master.h
 Description:    			TWI Master functions include file
 
 ********************************************************/
#ifndef _TWIM
#define _TWIM

#define TWIM_READ    1
#define TWIM_WRITE   0

#define TWI_READY 0
#define TWI_MRX   1
#define TWI_MTX   2
#define TWI_SRX   3
#define TWI_STX   4

#ifndef TWI_BUFFER_LENGTH
#define TWI_BUFFER_LENGTH 16
#endif

uint8_t	TWIM_Init (uint32_t TWI_Bitrate);
uint8_t TWIM_Start (uint8_t address, uint8_t TWIM_Type);
void	TWIM_Stop (void);
uint8_t	TWIM_Write (uint8_t byte);
uint8_t	TWIM_ReadAck (void);
uint8_t	TWIM_ReadNack (void);
uint8_t twi_readFrom(uint8_t address, uint8_t* data, uint8_t length);

#endif
