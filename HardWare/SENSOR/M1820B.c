#include "M1820B.h"
#include "delay.h"
#include "usart.h"


	u16 real_temp = 0x00,current_temp = 0x01, last_temp = 0x00;


void M1820B_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	DS_OUT = 1;
}

void DS_OUT_MODE(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void DS_IN_MODE(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}


void M1820B_Rest(void)
{
	u16 j = 0, i = 10000;
	DS_OUT = 1;
	delay_us(8);
	DS_OUT = 0;
	delay_us(600);
	DS_OUT = 1;
	delay_us(50);
	DS_IN_MODE();
	j = DS_IN;
	delay_xms(50);
	while(0 == j)
	{
		if(--i)
		{
			return;
		}
	}
	DS_OUT = 1;
	delay_us(600);
	//DS_OUT_MODE();
}

//写一个字节
void WriteOneChar(u8 Byte)
{
	u8 i = 0;
	DS_OUT_MODE();
	for(i = 0; i < 8; i++)
	{
		DS_OUT = 0;
		delay_us(2);
		DS_OUT = Byte & 0x01;
		delay_us(70);
		DS_OUT = 1;
		delay_us(2);
		Byte >>= 1;
	}
	
}

//读一个字节
u8 ReadOneChar(void)
{
	u8 i = 0, j = 0, data = 0;
	DS_OUT_MODE();
	for(i = 0; i < 8; i++)
	{
		DS_OUT = 0;
		delay_us(2);
		DS_OUT = 1;
		delay_us(6);
		DS_IN_MODE();
		j = DS_IN;
		delay_us(3);
		data = (data>>1)|(j << 7);
		delay_us(50);
		DS_OUT_MODE();
		DS_OUT = 1;
	}
	return data;
}


//读取温度
float Get_M1802B_Temp(void)
{
    u8 flag = 0, low=0,high = 0,transfer_temp = 0, transfer_temp2=0,transfer_rsh = 0;
	float temp = 0, temp_float = 0;
	
	//向M1820B发送指令
	M1820B_Rest();
//	delay_xms(30);
	WriteOneChar(0xCC);//跳过读序号列号的操作
	WriteOneChar(0x44);//启动温度转换

	M1820B_Rest();
	WriteOneChar(0xCC);  //跳过读序号列号的操作
	WriteOneChar(0xBE);  //读取温度寄存器
	/* 主机发布BE指令后，必须立即产生读时序 */
	low=ReadOneChar();     //读低8位
	high=ReadOneChar();     //读高8位
	current_temp = (high<<8)| low;
//	printf("current_temp = 0x%x\r\n",current_temp);
//	if(current_temp != 0 || current_temp != 65535)
//	{
//		last_temp = current_temp;
//		real_temp = current_temp;
//        printf("last_temp = 0x%x\r\n",last_temp);
//	}
    if(current_temp >0 && current_temp< 0xffff)
	{
		last_temp = current_temp;
		real_temp = current_temp;
//        printf("last_temp = 0x%x\r\n",last_temp);
	}
//	if(current_temp == 0 || current_temp == 0xffff)
    else
	{
		real_temp = last_temp;
	}
//	printf("real_temp = 0x%x\r\n",real_temp);
/* 开始对采集回来的数据进行处理 */
	temp_float = (real_temp & 0xFF) * 0.00390625; 	/*保留8位小数*/		 
	transfer_rsh =  (real_temp&0xFF00) >> 8;	    /*保留高8位整数,bit15 S为正负号*/		 
	/*补码转原码操作*/
	flag = (transfer_rsh&0x80)>>7; /* 判断数据的第15位 即确定正负数 */
	if(flag) /* 假如第15bit为1，即寄存器数据为正数 */
	{
		transfer_temp = transfer_rsh;
		transfer_temp2 = ~(transfer_temp)+ 1 ; //对数据取反+1
		if((transfer_temp2&0x40)>>6) temp += 64; else temp += 0;	 
		if((transfer_temp2&0x20)>>5) temp += 32; else temp += 0;
		if((transfer_temp2&0x10)>>4) temp += 16; else temp += 0;
		if((transfer_temp2&0x08)>>3) temp += 8;  else temp += 0;
		if((transfer_temp2&0x04)>>2) temp += 4;  else temp += 0;
		if((transfer_temp2&0x02)>>1) temp += 2;  else temp += 0;
		if((transfer_temp2&0x01)>>0) temp += 1;  else temp += 0;
		temp = -temp + 40;
	}
	else /* 假如第15bit为0，即寄存器数据为正数 */
	{
		transfer_temp2 = transfer_rsh; //0x6A
		if((transfer_temp2&0x40)>>6) temp += 64; else temp += 0;	 
		if((transfer_temp2&0x20)>>5) temp += 32; else temp += 0;
		if((transfer_temp2&0x10)>>4) temp += 16; else temp += 0;
		if((transfer_temp2&0x08)>>3) temp += 8;  else temp += 0;
		if((transfer_temp2&0x04)>>2) temp += 4;  else temp += 0;
		if((transfer_temp2&0x02)>>1) temp += 2;  else temp += 0;
		if((transfer_temp2&0x01)>>0) temp += 1;  else temp += 0;
		temp = temp + 40;
	}
	temp += temp_float;
 	return((u8)temp);
}




