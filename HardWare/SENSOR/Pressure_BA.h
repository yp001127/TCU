#ifndef __PRESSURE_BA_H
#define __PRESSURE_BA_H

#include "sys.h"


void HP5835_BA_WriteReg(uint8_t addr, uint8_t data);
uint8_t Read_BA_Byte(uint8_t addr);
void HP5835_BA_Init(void);
void ReadPressure_BA(int32_t* PressBuf);
#endif

