#ifndef __PROCESS_DT_H
#define __PROCESS_DT_H

#include "sys.h"


#define DATA_MAX_SIZE 20


typedef struct
{
	float temp1;
	int32_t PressBuf;
	int32_t PressBuf2;
	uint16_t Volt;
	uint16_t Speed;
	uint8_t Volt_State;
	uint8_t Temp_Sensor_State;
	uint8_t Light_State;
	uint8_t Motor_State;
	uint8_t BoardTemp;
	uint16_t EcsCurrent;
	uint16_t LightCurrent;
}SensorDataTypeDef;

uint8_t TX_SumCheck(const uint8_t *buf, uint8_t len);
void ReadAllSensor_(float *temp1, uint8_t *SensorState, int32_t* PressBuf, int32_t* PressBuf2,u8* BoardTemp, u16 *EcsCurrent, u16 *LightCurrent);



#endif
