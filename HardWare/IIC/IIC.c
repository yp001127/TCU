//#include "IIC.h"
//#include "delay.h"

//uint8_t F_VarIT_I2cAck;
////初始化IIC
//void IIC_Init(void)
//{					     
////	GPIO_InitTypeDef GPIO_InitStructure;
////	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
////	   
////	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;
////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
////	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
////	GPIO_Init(GPIOB, &GPIO_InitStructure);
//// 
////	IIC_SCL=1;
////	IIC_SDA=1;

//}
////产生IIC起始信号
//void IIC_Start(void)
//{
//	SDA_OUT();     //sda线输出
//	IIC_SDA=1;
//	delay_us(20);
//	IIC_SCL=1;
//	delay_us(20);
// 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
//	delay_us(20);
//	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
//	delay_us(18);
//}	  
////产生IIC停止信号
//void IIC_Stop(void)
//{
//	SDA_OUT();//sda线输出
//	IIC_SCL=0;
//	delay_us(20);
//	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
// 	delay_us(20);
//	IIC_SCL=1;
//	delay_us(20);	
//	IIC_SDA=1;//发送I2C总线结束信号
//	delay_us(18);							   	
//}
////等待应答信号到来
////返回值：1，接收应答失败
////        0，接收应答成功
//void IIC_Wait_Ack(void)
//{
//	IIC_SDA = 0;
//	delay_us(20);
//	IIC_SCL = 1;
//	delay_us(20);
//	IIC_SDA = 1;
//	delay_us(18);	
//} 
////产生ACK应答
//void IIC_Ack(void)
//{
//	delay_us(2);
//	//IIC_SCL=0;
//	SDA_OUT();
//	IIC_SDA=0;
//	delay_us(20);
//	IIC_SCL=1;
//	delay_us(20);
//	IIC_SCL=0;
//	delay_us(20);
//}
////不产生ACK应答		    
//void IIC_NAck(void)
//{
//	//IIC_SCL=0;
//	SDA_OUT();
//	IIC_SDA=1;
//	delay_us(20);
//	IIC_SCL=1;
//	delay_us(20);
//	IIC_SCL=0;
//	delay_us(20);
//}					 				     
//void IIC_Send_Byte(unsigned char databuf)
//{
//	unsigned char i;

//		delay_us(2);       // 防止两芯片输出冲突
//		SDA_OUT();		//SDA改为输出模式
//		for(i=0;i<8;i++)
//		{
//			if(databuf&0x80)
//			{
//				IIC_SDA = 1;
//				delay_us(19);
//			}
//			else
//			{
//				IIC_SDA = 0;
//				delay_us(18);
//			}
//			IIC_SCL = 1;
//			databuf <<= 1;
//			delay_us(20);
//			IIC_SCL = 0;
//		}
//		delay_us(2);       // 防止两芯片输出冲突
//		SDA_IN(); 		//SDA改为输入模式
//		delay_us(19);
//		F_VarIT_I2cAck = 0;
//		IIC_SCL = 1;			//ACK脉冲
//		for(i = 0;i < 20; i++)
//		{
//			if(!READ_SDA)
//			{
//				F_VarIT_I2cAck = 1;
//				break;
//			}
//		}
//		delay_us(19);
//		IIC_SCL = 0;
//		delay_us(2);       // 防止两芯片输出冲突
//		SDA_OUT();		//SDA改为输出模式
//		IIC_SDA = 1;
//}

///*******************************************************************
//---Function Description:  read i2c data	0最后1字节 非0为非最后1字节
//*******************************************************************/
//unsigned char IIC_Read_Byte(unsigned char endbyte)
//{
//	unsigned char xx;
//	unsigned char i;

//	delay_us(10);
//	SDA_IN();		//SDA改为输入模式
//	delay_us(10);
//	for(i=0;i<8;i++)
//	{
//		xx = xx << 1;
//		IIC_SCL = 1;
//		delay_us(19);
//		if(READ_SDA == 1)
//		{
//			 xx |= 0x01;
//		}
//		else
//		{
//			 xx |= 0x00;
//		}
//		IIC_SCL = 0;
//		delay_us(20);
//	}

//	delay_us(2);       // 防止两芯片输出冲突
//	SDA_OUT();	  //SDA改为输出模式
//	
//	if(endbyte != 0)
// 		IIC_Ack();
// 	else
//  		IIC_NAck();
//	return xx;
//}







































