#include "Temp.h"
#include "SPI.h"
#include "delay.h"
#include "usart.h"
#include "Process_DT.h"

void ReadMAX31855(float *temp1, uint8_t *SensorState)
{
	//uint32_t count;
	uint16_t t1;				//热电偶温度临时变量
//	uint8_t SensorState;	//热电偶故障代码
//	uint16_t t2;				//参考端内部临时变量
	uint8_t i;
	uint8_t data[4] = {0};	//	SPI数据存放临时变量
	//先拉高初始化片选信号，180ms后在进行读数据
	MySPI_Stop();				//CS = 1
	delay_xms(180);
	
	MySPI_Start(); 			//CS = 0
	for(i = 0;i < 4; i++)
	{
		data[i] = MySPI_ReadByte();
	}
	MySPI_Stop();			//CS = 1
	
	t1 = data[0];			//将31-24数据放在t1中
	//温度为负
	if((t1 & 0x80) == 1)
	{
		t1 = (t1 << 8) | data[1];
		t1 = ~t1 + 0x01;
		*temp1 = (t1 >> 2) * 0.25;	//获取热电偶温度
	}
	else
	{
		t1 = (t1 << 8) | data[1];
		*temp1 = (t1 >> 2) * 0.25;	//获取热电偶温度
	}
	if(data[1] & 0x01)
	{
		*SensorState = 1;
	}
	else
	{
		*SensorState = 4;
	}
//	//热电偶开路
//	if(data[3] & 0x01 >> 0)
//	{
//		*SensorState = 1;
//	}
//	//热电偶与GND短路
//	if((data[3] & 0x02) >> 1)
//	{
//		*SensorState = 2;
//	}
//	//热电偶与VCC短路
//	if((data[3] & 0x04) >> 2)
//	{
//		*SensorState = 3;
//	}
//	if((data[3] & 0x07) == 0)
//	{
//		*SensorState = 4;
//	}
//	//冷端温度
//	t2 = data[2];
//	if((t2 & 0x80) == 1)
//	{
//		t2 = (t2 << 8) | data[3];
//		t2 = ~t2 + 1;
//		*temp2 = (t2 >> 4) * 0.0625;
//	}
//	else
//	{
//		t2 = (t2 << 8) | data[3];
//		*temp2 = (t2 >> 4) * 0.0625;		//获取冷端温度
//	}
//	
}	

