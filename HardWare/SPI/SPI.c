#include "SPI.h"
#include "delay.h"

/**
****************************************************************

****************************************************************
**/

//片选电平
void MySPI_W_SS(uint8_t Byte)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_8, (BitAction)Byte);
}

//时钟电平
void MySPI_W_SCK(uint8_t Byte)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)Byte);
}

////主机发送到从机
//void MySPI_W_MOSI(uint8_t Byte)
//{
//	GPIO_WriteBit(GPIOA, GPIO_Pin_7, (BitAction)Byte);
//}

//从机发送到主机
uint8_t MySPI_R_MISO()
{
	return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5);

}

//初始化
void MySPI_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	MySPI_W_SS(1);
	MySPI_W_SCK(0);
}

//开始
void MySPI_Start(void)
{
	MySPI_W_SS(0);
}

//结束
void MySPI_Stop(void)
{
	MySPI_W_SS(1);
}

//交换字节(只收不发)注意延时
uint8_t MySPI_ReadByte(void)
{
	uint8_t ReceiveByte = 0x00, i;
	
	for(i = 0; i < 8; i++)
	{
		MySPI_W_SCK(1);
		delay_ms(1);
		if(MySPI_R_MISO() == 1)
			ReceiveByte |= (0x80 >> i);	//主接收字节
		MySPI_W_SCK(0);
		delay_ms(1);
	}
	return ReceiveByte;
}
