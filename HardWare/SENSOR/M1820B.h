#ifndef __M1820B_H
#define __M1820B_H

#include "sys.h"


#define DS_OUT  PBout(0)
#define DS_IN  	PBin(0)

void M1820B_GPIO_Init(void);
void M1820B_Rest(void);
void WriteOneChar(u8 Byte);
u8 ReadOneChar(void);
float Get_M1802B_Temp(void);
void DS_OUT_MODE(void);
void DS_IN_MODE(void);


#endif

