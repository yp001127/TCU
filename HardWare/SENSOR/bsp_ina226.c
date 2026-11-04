#include "bsp_ina226.h"
#include "bsp_i2c.h"
#include "delay.h"


/////读数据
uint16_t INA226_Read2Byte(uint16_t cmd, uint8_t reg_addr)
{
	uint16_t reg_data=0;
	uint16_t temp=0;
	IIC_Start();
	IIC_Send_Byte(cmd);
	if(IIC_Wait_Ack())return 0;   
	IIC_Send_Byte(reg_addr);   
	if(IIC_Wait_Ack())return 0;   
	IIC_Start();
	IIC_Send_Byte(cmd+1);
	if(IIC_Wait_Ack())return 0;   
	reg_data= IIC_Read_Byte(1);
	reg_data=(reg_data<<8)&0xFF00;
	temp=IIC_Read_Byte(0);
	IIC_Stop();
	reg_data|=temp;
	return reg_data;
}


//写数据
uint8_t INA226_Write2Byte(uint16_t cmd, uint8_t reg_addr,uint16_t reg_data)
{        
	uint8_t data_high=(uint8_t)((reg_data&0xFF00)>>8);
	uint8_t data_low=(uint8_t)reg_data&0x00FF;
	IIC_Start();
	IIC_Send_Byte(cmd);   
	if(IIC_Wait_Ack())return 0;
	IIC_Send_Byte(reg_addr );    
	if(IIC_Wait_Ack())return 0;        
	IIC_Send_Byte(data_high);
	if(IIC_Wait_Ack())return 0;        
	IIC_Send_Byte(data_low);
	if(IIC_Wait_Ack())return 0;                 
	IIC_Stop();
	delay_xms(2);
	return 1;
}

//初始化
void INA226_Init(uint16_t Cmd)
{
	IIC_Init();
    uint16_t temp;
    temp =
	(1<<14)|
	(0x02<<9)|  //采样平均次数  n=4^i 次平均
	(0x02<<6)|  //总线电压转换时间 0.588 ms
	(0x02<<3)|  //并联电压采样时间 1.1 ms
	(0x07<<0);  //模式  电流和电压连续采样
    
	INA226_Write2Byte(Cmd, Config_Reg, temp);
	INA226_Write2Byte(Cmd, Calib_Reg, (25600));//0.00512f/(109.5445f/32768.0f*0.0005f0.00512/(Current_LSB*R_SHUNT) = 512  //电流偏大改小
}

