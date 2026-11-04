#include "Pressure.h"
#include "IOIIC2.h"
#include "delay.h"
#include "usart.h"
/**
************************************************
#define HP5834_RST_ADDR				0x06
#define HP5834_READ_PT_ADDR 	0x10
#define HP5834_READ_P_ADDR 		0x30
#define HP5834_READ_T_ADDR 		0x32
#define HP5834_ANA_CAL_ADDR 	0x28
#define HP5834_WRITE_ADDR			0xEC
#define HP5834_READ_ADDR			0xED
***********************************************
**/
/*向HP5834写入命令*/
void HP5834_WriteReg(uint8_t addr, uint8_t data)
{	
	uint8_t acktemp = 1;
	IOIIC2_Start();
	IOIIC2_Send_Byte(0xDA);
	acktemp = IOIIC2_Wait_Ack();	
	IOIIC2_Send_Byte(addr);
	acktemp = IOIIC2_Wait_Ack();	
	IOIIC2_Send_Byte(data);
	acktemp = IOIIC2_Wait_Ack();
	IOIIC2_Stop();
}
uint8_t Read_Byte(uint8_t addr)
{
	uint8_t acktemp;
	uint8_t mydata;
	
	IOIIC2_Start();
	IOIIC2_Send_Byte(0xDA);
	acktemp = IOIIC2_Wait_Ack();
	IOIIC2_Send_Byte(addr);
	acktemp = IOIIC2_Wait_Ack();
	IOIIC2_Start();
	IOIIC2_Send_Byte(0xDB);
	acktemp = IOIIC2_Wait_Ack();
	mydata = IOIIC2_Read_Byte();
	IOIIC2_Stop();
	
	return mydata;
}
/***初始化HP5834***/
void HP5834_Init(void)
{	
	IOIIC2_Init();
	delay_ms(1);
}

/*******************单独读取气压********************************************/
void ReadPressure(int32_t* PressBuf)
{
	uint8_t pressure_H, pressure_M, pressure_L;
	uint8_t temp_a5;
	int32_t pressure_adc;
	float pressure;		
	temp_a5 = Read_Byte(0xA5);
	temp_a5=(temp_a5 & 0xF0)| 0x05;
	HP5834_WriteReg(0xA5, temp_a5);
	HP5834_WriteReg(0x30, 0x0A);
	//while(((Read_Byte(0x30))&0x80) > 0);
	pressure_H = Read_Byte(0X06);
	pressure_M = Read_Byte(0X07);
	pressure_L = Read_Byte(0X08);
	delay_xms(10);
	pressure_adc = pressure_H * 65536 + pressure_M * 256 + pressure_L;
	
	//负压处理
	if (pressure_adc > 8388608)
	{
		pressure = (float)(pressure_adc - 16777216) / 8 / 1000;
        if(pressure < 0)
        {
            pressure = -pressure;
        }
	}
	else
	{
		pressure = (float)pressure_adc / 8 /1000;
	}
	pressure = pressure * 100;
	*PressBuf = (int32_t)pressure;
}
