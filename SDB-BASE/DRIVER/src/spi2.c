
#include "stm32f10x.h"
#include "pindef.h"


////////////////////////////////////////////////////////////////////////////////
unsigned char SPI2_SendByte(uint8_t byte)
{
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI2, byte);         
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);      
	return SPI_I2S_ReceiveData(SPI2);
}

////////////////////////////////////////////////////////////////////////////////
void SPI2_TXByte(unsigned char byte)
{
	  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);       

	  SPI_I2S_SendData(SPI2, byte);	
}
