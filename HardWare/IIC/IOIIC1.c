#include "IOIIC1.h"
#include "delay.h"


#define delay_time1 6
uint8_t F_VarIT_I2cAck1;
//初始化IOIIC1
void IOIIC1_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
 
	IOIIC1_Stop();
}

void IOIIC_SDA_OUT1(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void IOIIC_SDA_IN1(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
//产生IOIIC1起始信号
void IOIIC1_Start(void)
{
	IOIIC_SDA_OUT1();     //sda线输出
	IOIIC1_SDA=1;
	delay_us(delay_time1);
	IOIIC1_SCL=1;
	delay_us(delay_time1);
 	IOIIC1_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(delay_time1);
	IOIIC1_SCL=0;//钳住I2C总线，准备发送或接收数据 
	delay_us(delay_time1);
}	  
//产生IOIIC1停止信号
void IOIIC1_Stop(void)
{
	IOIIC_SDA_OUT1();//sda线输出
	IOIIC1_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(delay_time1);
	IOIIC1_SCL=1;
	delay_us(delay_time1);	
	IOIIC1_SDA=1;//发送I2C总线结束信号
	delay_us(delay_time1);	
	IOIIC1_SCL=0;
	delay_us(delay_time1);	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t IOIIC1_Wait_Ack(void)
{
	IOIIC_SDA_OUT1();//sda线输出
	IOIIC1_SDA = 1;
	delay_us(delay_time1);
	IOIIC1_SCL = 1;
	delay_us(delay_time1/2);
	IOIIC_SDA_IN1();
	F_VarIT_I2cAck1 = IOIIC_READ_SDA1;
	delay_us(delay_time1/2);
	IOIIC1_SCL = 0;
	delay_us(delay_time1);	
	if(F_VarIT_I2cAck1 == 1)
	{
		return 0;
	}
	return 1;
} 
////产生ACK应答
//void IOIIC1_Ack(void)
//{
//	delay_us(2);
//	//IOIIC1_SCL=0;
//	IOIIC_SDA_OUT1();
//	IOIIC1_SDA=0;
//	delay_us(600);
//	IOIIC1_SCL=1;
//	delay_us(600);
//	IOIIC1_SCL=0;
//	delay_us(600);
//}
////不产生ACK应答		    
//void IOIIC1_NAck(void)
//{
//	//IOIIC1_SCL=0;
//	IOIIC_SDA_OUT1();
//	IOIIC1_SDA=1;
//	delay_us(600);
//	IOIIC1_SCL=1;
//	delay_us(600);
//	IOIIC1_SCL=0;
//	delay_us(600);
//}					 				     
void IOIIC1_Send_Byte(unsigned char databuf)
{
	char i;
	IOIIC_SDA_OUT1();//sda线输出
	for(i=0;i<8;i++)
	{
		if((databuf<<i)&0x80)
		{
			IOIIC1_SDA = 1;
			delay_us(delay_time1);
			IOIIC1_SCL = 1;
			delay_us(delay_time1);
			IOIIC1_SCL = 0;
			delay_us(delay_time1);
		}
		else
		{
			IOIIC1_SDA = 0;
			delay_us(delay_time1);
			IOIIC1_SCL = 1;
			delay_us(delay_time1);
			IOIIC1_SCL = 0;
			delay_us(delay_time1);
		}	
	}
}

/*******************************************************************
---Function Description:  read i2c data	0最后1字节 非0为非最后1字节
*******************************************************************/
uint8_t IOIIC1_Read_Byte(void)
{
	uint8_t b=0,i;
	
	for(i=0;i<8;i++)
	{
		IOIIC_SDA_OUT1();//sda线输出
		IOIIC1_SDA = 1;
		delay_us(delay_time1);
		IOIIC1_SCL = 1;
		delay_us(delay_time1);
		IOIIC_SDA_IN1();
		F_VarIT_I2cAck1 = IOIIC_READ_SDA1;
		delay_us(delay_time1);
		IOIIC1_SCL = 0;
		if(F_VarIT_I2cAck1 == 1)
		{
			b = b << 1;
			b = b | 0x01;
		}
		else
		{
			b = b << 1;
		}
	}
	return b;
}

