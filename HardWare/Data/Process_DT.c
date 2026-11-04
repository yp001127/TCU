#include "Process_DT.h"
#include "Temp.h"
#include "Pressure.h"
#include "delay.h"
#include "HP5834_BA.h"
#include "Pressure_BA.h"
#include "M1820B.h"
#include "bsp_ina226.h"
//读取温度，压力数据
void ReadAllSensor_(float *temp1, uint8_t *SensorState, int32_t* PressBuf, int32_t* PressBuf2,u8* BoardTemp, u16 *EcsCurrent, u16 *LightCurrent)
{
	ReadPressure(PressBuf);
	//delay_ms(20);
	ReadPressure_BA(PressBuf2);
	//delay_ms(20);
	ReadMAX31855(temp1, SensorState);
	//delay_ms(20);
	//板子温度
	*BoardTemp = Get_M1802B_Temp();
	//电调电流
	*EcsCurrent = (INA226_Read2Byte(WRITE_ADDR_0X85, Shunt_V_Reg))*25600*0.15/2048;
	//点火器电流
	*LightCurrent=(INA226_Read2Byte(WRITE_ADDR, Shunt_V_Reg))*25600*0.2*0.30/2048;
    if(*LightCurrent>45000) *LightCurrent=0;
    
}
//和校验
uint8_t TX_SumCheck(const uint8_t *buf, uint8_t len)
{
	uint8_t i, ret = 0;
	
	for(i = 0; i < len; i++)
	{
		ret += buf[i];
	}
	ret = ~ret;
	
	return ret;
}
