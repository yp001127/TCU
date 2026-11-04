#ifndef __IOIIC2_H
#define __IOIIC2_H
#include "sys.h"
	  
//IO方向设置
//#define IOIIC_SDA_IN2()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;GPIOB->ODR |= (1 << 7);}
//#define IOIIC_SDA_IN2()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=8<<28;}
//#define IOIIC_SDA_OUT2() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=3<<28;}

//IO操作函数	 
#define IOIIC2_SCL    PBout(6) //SCL
#define IOIIC2_SDA    PBout(7) //SDA	 
#define IOIIC_READ_SDA2   PBin(7)  //输入SDA 


//IOIIC2所有操作函数
void IOIIC2_Init(void);                //初始化IOIIC2的IO口				 
void IOIIC2_Start(void);				//发送IOIIC2开始信号
void IOIIC2_Stop(void);	  			//发送IOIIC2停止信号
void IOIIC2_Send_Byte(unsigned char databuf);			//IOIIC2发送一个字节
uint8_t IOIIC2_Read_Byte(void);//IOIIC2读取一个字节
uint8_t IOIIC2_Wait_Ack(void); 				//IOIIC2等待ACK信号
//void IOIIC2_Ack(void);					//IOIIC2发送ACK信号
//void IOIIC2_NAck(void);				//IOIIC2不发送ACK信号

#endif
