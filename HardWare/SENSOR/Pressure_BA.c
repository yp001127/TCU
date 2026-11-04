#include "Pressure_BA.h"
#include "IOIIC1.h"
#include "delay.h"
#include "usart.h"

/*向HP5834写入命令*/
void HP5835_BA_WriteReg(uint8_t addr, uint8_t data)
{	
	uint8_t acktemp = 1;
	IOIIC1_Start();
	IOIIC1_Send_Byte(0xDA);
	acktemp = IOIIC1_Wait_Ack();	
	IOIIC1_Send_Byte(addr);
	acktemp = IOIIC1_Wait_Ack();	
	IOIIC1_Send_Byte(data);
	acktemp = IOIIC1_Wait_Ack();
	IOIIC1_Stop();
}
uint8_t Read_BA_Byte(uint8_t addr)
{
	uint8_t acktemp;
	uint8_t mydata;
	
	IOIIC1_Start();
	IOIIC1_Send_Byte(0xDA);
	acktemp = IOIIC1_Wait_Ack();
	IOIIC1_Send_Byte(addr);
	acktemp = IOIIC1_Wait_Ack();
	IOIIC1_Start();
	IOIIC1_Send_Byte(0xDB);
	acktemp = IOIIC1_Wait_Ack();
	mydata = IOIIC1_Read_Byte();
	IOIIC1_Stop();
	
	return mydata;
}
/***初始化HP5834***/
void HP5835_BA_Init(void)
{	
	IOIIC1_Init();
	delay_ms(1);
}

/*******************单独读取气压********************************************/
void ReadPressure_BA(int32_t* PressBuf)
{
	uint8_t pressure_H, pressure_M, pressure_L;
	uint8_t temp_a5;
	int32_t pressure_adc;
	float pressure;		
	temp_a5 = Read_BA_Byte(0xA5);
	temp_a5=(temp_a5 & 0xF0)| 0x05;
	HP5835_BA_WriteReg(0xA5, temp_a5);
	HP5835_BA_WriteReg(0x30, 0x0A);
	//while(((Read_BA_Byte(0x30))&0x80) > 0);
	pressure_H = Read_BA_Byte(0X06);
	pressure_M = Read_BA_Byte(0X07);
	pressure_L = Read_BA_Byte(0X08);
	
	pressure_adc = pressure_H * 65536 + pressure_M * 256 + pressure_L;
	
	//负压处理
	if (pressure_adc > 8388608)
	{
		pressure = (float)(pressure_adc - 16777216) / 8 / 1000;
	}
	else
	{
		pressure = (float)pressure_adc / 8 /1000;
	}
	pressure = pressure * 100;
	*PressBuf = (int32_t)pressure;
}
