#include "can.h"
#include "delay.h"
/*在中断处理函数中返回*/
//__IO uint32_t ret = 0;

//接收数据缓冲器
uint8_t flag = 0;
CanRxMsg RxMessage;				 //接收缓冲区
CanTxMsg TxMessage;

void CAN1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; 
	  CAN_InitTypeDef        CAN_InitStructure;
 	  CAN_FilterInitTypeDef  CAN_FilterInitStructure;
   	NVIC_InitTypeDef  NVIC_InitStructure;


	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能PORTA时钟	                   											 

  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//使能CAN1时钟	

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽
    GPIO_Init(GPIOA, &GPIO_InitStructure);		//初始化IO
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化IO
	  
 	//CAN单元设置
 	  CAN_InitStructure.CAN_TTCM=DISABLE;						 //非时间触发通信模式  //
 	  CAN_InitStructure.CAN_ABOM=DISABLE;						 //软件自动离线管理	 //
  	CAN_InitStructure.CAN_AWUM=DISABLE;						 //睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)//
  	CAN_InitStructure.CAN_NART=ENABLE;						 	//禁止报文自动传送 //
  	CAN_InitStructure.CAN_RFLM=DISABLE;						 //报文不锁定,新的覆盖旧的 // 
  	CAN_InitStructure.CAN_TXFP=DISABLE;						 //优先级由报文标识符决定 //
  	CAN_InitStructure.CAN_Mode= CAN_Mode_Normal;	         //模式设置： mode:0,普通模式;1,回环模式; //
  	//设置波特率
  	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;				//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位  CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=CAN_BS1_5tq; //Tbs1=tbs1+1个时间单位CAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=CAN_BS2_2tq;//Tbs2=tbs2+1个时间单位CAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=9;            //分频系数(Fdiv)为brp+1	//
  	CAN_Init(CAN1, &CAN_InitStructure);            // 初始化CAN1 

 	  CAN_FilterInitStructure.CAN_FilterNumber=0;	  //过滤器0
   	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32位 
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32位ID
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32位MASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//过滤器0关联到FIFO0
 	  CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //激活过滤器0

  	CAN_FilterInit(&CAN_FilterInitStructure);//滤波器初始化

	
	  CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0消息挂号中断允许.		    
  
  	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // 主优先级为1
//  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // 次优先级为0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);

}   																															//使能FIFO0消息挂号中断		
/*
 * 函数名：CAN_Send_Msg
 * 描述  ：CAN通信报文内容设置,设置一个数据内容为0-7的数据包
 * 输入  ：发送报文结构体
 * 输出  : 无
 * 调用  ：外部调用
 */	
uint8_t CAN_Send_Msg(const SensorDataTypeDef* msg)
{
//	uint8_t mailbox;
	char data[25] = {0};
	uint16_t i = 0;
	uint8_t packet_num,y;
//  TxMessage.StdId=0x00;						 
  TxMessage.ExtId=0x1314;					 //使用的扩展ID
  TxMessage.IDE=CAN_ID_EXT;					 //扩展模式
  TxMessage.RTR=CAN_RTR_DATA;				 //发送的是数据
  TxMessage.DLC=8;							 //数据长度为len字节
//	sprintf(data, "%.1f %2d %2d %2d %2d %2d %2d %2d ",
//					msg->temp1,msg->Volt_State,
//					msg->PressBuf,msg->Light_State,
//					msg->PressBuf2,msg->Motor_State,
//					msg->Volt,msg->Speed);
	data[0] = (uint32_t)(msg->temp1 * 100) >> 24;
	data[1] = (uint32_t)(msg->temp1 * 100) >> 16;
	data[2] = (uint32_t)(msg->temp1 * 100) >> 8;
	data[3] = (uint32_t)(msg->temp1 * 100);
	data[4] = msg->Temp_Sensor_State;
	
	data[5] = msg->PressBuf >> 24;
	data[6] = msg->PressBuf >> 16;
	data[7] = msg->PressBuf >> 8;
	data[8] = msg->PressBuf;
	data[9] = msg->Light_State;
	
	data[10] = msg->PressBuf2 >> 24;
	data[11] = msg->PressBuf2 >> 16;
	data[12] = msg->PressBuf2 >> 8;
	data[13] = msg->PressBuf2;
	data[14] = msg->Motor_State;
	
	data[15] = msg->Volt >> 8;
	data[16] = msg->Volt;
	data[17] = msg->Speed >> 8;
	data[18] = msg->Speed;
	data[19] = msg->Volt_State;
	
	data[20] = msg->LightCurrent >> 8;
	data[21] = msg->LightCurrent;	
	data[22] = msg->EcsCurrent >> 8;
	data[23] = msg->EcsCurrent;
	data[24] = msg->BoardTemp;
	
	for(packet_num = 1; packet_num <= 5; packet_num++)
	{
		TxMessage.Data[0] = 0x04;
		TxMessage.Data[1] = packet_num;
		for(y = 0; y < 5; y++)
		{
			TxMessage.Data[y + 2] = data[y + i];
		}
		i += 5;
		uint8_t checksum = TX_SumCheck(TxMessage.Data, 7);
		TxMessage.Data[7] = checksum;
		delay_xms(1);
		CAN_Transmit(CAN1, &TxMessage);
	}

//	while((CAN_TransmitStatus(CAN1, mailbox) == CAN_TxStatus_Failed) && (i < 0XFFF))
//	{
//		i++;					//等待发送结束
//	}
//		
//	if(i >= 0XFFF)
//		return 1;
	return 0;
}

////接收函数
//uint8_t Can_Receive_Msg(uint8_t* buf)
//{
//	uint16_t i;
//	//没有数据直接返回零
//	if(CAN_MessagePending(CAN1,CAN_FIFO0) == 0)
//		return 0;
//	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//读取数据
//	for(i = 0; i < 8; i++)
//		buf[i] = RxMessage.Data[i];
//	return RxMessage.DLC;
//}


///*
// * 函数名：USB_LP_CAN1_RX0_IRQHandler
// * 描述  ：USB中断和CAN接收中断服务程序，USB跟CAN公用I/O，这里只用到CAN的中断。
// * 输入  ：无
// * 输出  : 无	 
// * 调用  ：无
// */
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	/*从邮箱中读出报文*/
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	/* 比较ID是否为0x1314 */ 
	if((RxMessage.ExtId==0x1314) && (RxMessage.IDE==CAN_ID_EXT) && (RxMessage.DLC==8) )
	{
		flag = 1; 					       //接收成功  
	}
	else
	{
		flag = 0; 					   //接收失败
	}
}








