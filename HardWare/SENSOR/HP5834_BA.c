//#include "HP5834_BA.h"
//#include "delay.h"
//#include "usart.h"
//#include "IIC2.h"
///**
//************************************************
//#define HP5834_RST_ADDR				0x06
//#define HP5834_READ_PT_ADDR 	0x10
//#define HP5834_READ_P_ADDR 		0x30
//#define HP5834_READ_T_ADDR 		0x32
//#define HP5834_ANA_CAL_ADDR 	0x28
//#define HP5834_WRITE_ADDR			0xEC
//#define HP5834_READ_ADDR			0xED
//***********************************************
//**/

//extern uint8_t F_VarIT_I2cAck1;
///*向HP5834写入命令*/

//void HP5834_BA_WriteReg(uint8_t Cmd)
//{	
//	
//	IIC2_Start();
//	IIC2_Send_Byte(0xEC);
//	if(F_VarIT_I2cAck1 == 0)
//	{
//		IIC2_Wait_Ack();
//		return;
//	}
//	IIC2_Send_Byte(Cmd);
//	if(F_VarIT_I2cAck1 == 0)
//	{
//		IIC2_Wait_Ack();
//		return;
//	}
//	IIC2_Stop();
//}
///***初始化HP5834***/
//void HP5834_BA_Init(void)
//{	
//	IIC2_Init();
//	HP5834_BA_WriteReg(HP5834_RST_ADDR);
//	delay_ms(5);
//}
///***********************读取压力和温度**************************************/
//void HP5834_BA_ReadPressureAndTemperature(int* TemperatureBuf2, unsigned int* PressBuf2)
//{
//	unsigned long temp_Buf;
//	HP5834_BA_WriteReg(0x28);
//	HP5834_BA_WriteReg(0x40|0x08);				//采样率 1024
//	delay_ms(45);				//delay 45ms以上

////	Hp20X_WriteCmd(0x40|0x00);				//采样率 4096
////	Delay_xms(150);				//delay 150ms

//	HP5834_BA_WriteReg(0x10);
//	IIC2_Start();
//	IIC2_Send_Byte(0xed);
//	if(F_VarIT_I2cAck1 == 0)
//	{
//		IIC2_Wait_Ack();
//		return;
//	}
//	temp_Buf = IIC2_Read_Byte(1);
//	temp_Buf <<= 8;
//	temp_Buf |= IIC2_Read_Byte(1);
//	temp_Buf <<= 8;
//	temp_Buf |= IIC2_Read_Byte(1);
//	if(temp_Buf & 0x800000)
//		temp_Buf |= 0xff000000;

//	*TemperatureBuf2 = temp_Buf;
//	
//	temp_Buf = IIC2_Read_Byte(1);
//	temp_Buf <<= 8;
//	temp_Buf |= IIC2_Read_Byte(1);
//	temp_Buf <<= 8;
//	temp_Buf |= IIC2_Read_Byte(0);
//	IIC2_Stop();
//	*PressBuf2 = temp_Buf;
//}
///*******************单独读取气压********************************************/
//void HP5834_BA_ReadPressure(uint32_t* PressBuf2)
//{
//	unsigned long temp_Buf;
//	
//	HP5834_BA_WriteReg(0x40|0x08);				//采样率 1024
//	delay_ms(45);				//delay 45ms以上

////	Hp20X_WriteCmd(0x40|0x00);				//采样率 4096
////	Delay_xms(150);				//delay 150ms

//	HP5834_BA_WriteReg(0x30);// 
//	IIC2_Start();
//	IIC2_Send_Byte(0xed);
//	
//	temp_Buf = IIC2_Read_Byte(1);
//	temp_Buf <<= 8;
//	temp_Buf |= IIC2_Read_Byte(1);
//	temp_Buf <<= 8;
//	temp_Buf |= IIC2_Read_Byte(0);
//	IIC2_Stop();
//	if(temp_Buf & 0x800000)
//	{
//		temp_Buf |= 0xff000000;
//	}
//	*PressBuf2 = temp_Buf;
//}
