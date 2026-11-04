#ifndef __SPI_H
#define __SPI_H

#include "sys.h"

void MySPI_Init(void);
void MySPI_Start(void);
void MySPI_Stop(void);
uint8_t MySPI_ReadByte(void);
void MySPI_W_SCK(uint8_t Byte);
uint8_t MySPI_R_MISO(void);

#endif
