#include "IOIIC2.h"
#include "delay.h"

#define delay_time2 6
uint8_t F_VarIT_I2cAck2;
//初始化IOIIC2
void IOIIC2_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
 
	IOIIC2_Stop();
}

void IOIIC_SDA_OUT2(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void IOIIC_SDA_IN2(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
//产生IOIIC2起始信号
void IOIIC2_Start(void)
{
	IOIIC_SDA_OUT2();     //sda线输出
	IOIIC2_SDA=1;
	delay_us(delay_time2);
	IOIIC2_SCL=1;
	delay_us(delay_time2);
 	IOIIC2_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(delay_time2);
	IOIIC2_SCL=0;//钳住I2C总线，准备发送或接收数据 
	delay_us(delay_time2);
}	  
//产生IOIIC2停止信号
void IOIIC2_Stop(void)
{
	IOIIC_SDA_OUT2();//sda线输出
	IOIIC2_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(delay_time2);
	IOIIC2_SCL=1;
	delay_us(delay_time2);	
	IOIIC2_SDA=1;//发送I2C总线结束信号
	delay_us(delay_time2);	
	IOIIC2_SCL=0;
	delay_us(delay_time2);	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t IOIIC2_Wait_Ack(void)
{
	IOIIC_SDA_OUT2();//sda线输出
	IOIIC2_SDA = 1;
	delay_us(delay_time2);
	IOIIC2_SCL = 1;
	delay_us(delay_time2/2);
	IOIIC_SDA_IN2();
	F_VarIT_I2cAck2 = IOIIC_READ_SDA2;
	delay_us(delay_time2/2);
	IOIIC2_SCL = 0;
	delay_us(delay_time2);	
	if(F_VarIT_I2cAck2 == 1)
	{
		return 0;
	}
	return 1;
} 
////产生ACK应答
//void IOIIC2_Ack(void)
//{
//	delay_us(2);
//	//IOIIC2_SCL=0;
//	IOIIC_SDA_OUT2();
//	IOIIC2_SDA=0;
//	delay_us(600);
//	IOIIC2_SCL=1;
//	delay_us(600);
//	IOIIC2_SCL=0;
//	delay_us(600);
//}
////不产生ACK应答		    
//void IOIIC2_NAck(void)
//{
//	//IOIIC2_SCL=0;
//	IOIIC_SDA_OUT2();
//	IOIIC2_SDA=1;
//	delay_us(600);
//	IOIIC2_SCL=1;
//	delay_us(600);
//	IOIIC2_SCL=0;
//	delay_us(600);
//}					 				     
void IOIIC2_Send_Byte(unsigned char databuf)
{
	char i;
	IOIIC_SDA_OUT2();//sda线输出
	for(i=0;i<8;i++)
	{
		if((databuf<<i)&0x80)
		{
			IOIIC2_SDA = 1;
			delay_us(delay_time2);
			IOIIC2_SCL = 1;
			delay_us(delay_time2);
			IOIIC2_SCL = 0;
			delay_us(delay_time2);
		}
		else
		{
			IOIIC2_SDA = 0;
			delay_us(delay_time2);
			IOIIC2_SCL = 1;
			delay_us(delay_time2);
			IOIIC2_SCL = 0;
			delay_us(delay_time2);
		}	
	}
}

/*******************************************************************
---Function Description:  read i2c data	0最后1字节 非0为非最后1字节
*******************************************************************/
uint8_t IOIIC2_Read_Byte(void)
{
	uint8_t b=0,i;
	
	for(i=0;i<8;i++)
	{
		IOIIC_SDA_OUT2();//sda线输出
		IOIIC2_SDA = 1;
		delay_us(delay_time2);
		IOIIC2_SCL = 1;
		delay_us(delay_time2);
		IOIIC_SDA_IN2();
		F_VarIT_I2cAck2 = IOIIC_READ_SDA2;
		delay_us(delay_time2);
		IOIIC2_SCL = 0;
		if(F_VarIT_I2cAck2 == 1)
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

