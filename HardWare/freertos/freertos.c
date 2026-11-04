#include "stdio.h"
#include "string.h"
#include <stdlib.h> 

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"
#include "semphr.h"

#include "led.h"
#include "delay.h"
#include "sys.h"
#include "Pressure.h"
#include "adc.h"
#include "timer.h"
#include "can.h"
#include "SPI.h"
#include "Temp.h"
#include "lighter.h"
//#include "IIC.h"
#include "Process_DT.h"
#include "M1820B.h"
#include "bsp_ina226.h"
#include "config.h"
/************************************************
 各类定义以及全局变量
************************************************/
extern CanRxMsg RxMessage;				 //接收缓冲区
extern CanTxMsg TxMessage;				 //发送缓冲区
extern uint8_t flag;

uint16_t adcx = 0;
uint16_t volt = 0;

uint16_t counts = 0;
float temp;
u16 cur_value1 =0, cur_value2 =0;    
SensorDataTypeDef Sensor, *pSensor;

void start_task(void *pvParameters);


QueueHandle_t 			queue1 = NULL;
SemaphoreHandle_t		semaph_msg = NULL;
/*各任务句柄*/
TaskHandle_t LED_Task_Handler;
TaskHandle_t Sensor_Task_Handler;
TaskHandle_t Send_Task_Handler;
TaskHandle_t Control_Task_Handler;
TaskHandle_t Speed_Send_Task_Handler;
void Freertos_Init(void)
{
	//创建开始任务	
	xTaskCreate(start_task, "start_task", 288, NULL, 7, NULL);                      
}

/*系统指示灯，正常工作保持闪烁*/
void LED_Task(void *pvParameters)
{
	TickType_t	PreviousWakeTime = xTaskGetTickCount(); 
	while(1)
	{
		PreviousWakeTime = xTaskGetTickCount(); 
		LED0 =~LED0;
		vTaskDelayUntil(&PreviousWakeTime, configTICK_RATE_HZ/10);
	}
}

void Speed_Send_Task(void *pvParameters)
{
	TickType_t	PreviousWakeTime = xTaskGetTickCount();
    //uint8_t Can_Data[8] = {0};    
	while(1)
	{
		PreviousWakeTime = xTaskGetTickCount(); 
//        if(xSemaphoreTake(semaph_msg, (TickType_t)1000))
//        {
             if(xSemaphoreTake(semaph_msg, (TickType_t)1000))
            {
								delay_xms(1);
								TxMessage.ExtId=0x1314;					 //使用的扩展ID
								TxMessage.IDE=CAN_ID_EXT;					 //扩展模式
								TxMessage.RTR=CAN_RTR_DATA;				 //发送的是数据
								TxMessage.DLC=8;							 //数据长度为len字节
								TxMessage.Data[0] = 0x04;
								TxMessage.Data[1] = 0x06;
								TxMessage.Data[2] = 0x04;
								TxMessage.Data[3] = 0x04;
								TxMessage.Data[4] = ((Speed >> 8) & 0xff);
								TxMessage.Data[5] = Speed;
								TxMessage.Data[6] = 0x00;
								uint8_t checksum = TX_SumCheck(TxMessage.Data, 7);
								TxMessage.Data[7] = checksum;
            
                CAN_Transmit(CAN1, &TxMessage);
                xSemaphoreGive(semaph_msg);
            }
            
            delay_xms(5);
//            xSemaphoreGive(semaph_msg);
//        }
//        else
//        {
//            printf("failed to take sem in send msg\r\n");
//        }
		vTaskDelayUntil(&PreviousWakeTime, configTICK_RATE_HZ/1000);
	}
}

/*数据采集*/
void Sensor_Task(void *pvParameters)
{
	TickType_t	PreviousWakeTime = xTaskGetTickCount();
    
	while(1)
	{
		PreviousWakeTime = xTaskGetTickCount();
		ReadAllSensor_(&Sensor.temp1, &Sensor.Temp_Sensor_State, &Sensor.PressBuf, 
										&Sensor.PressBuf2, &Sensor.BoardTemp, &Sensor.EcsCurrent, &Sensor.LightCurrent);
//		adcx = Get_Adc_Average(ADC_Channel_9, 10);
//		volt= (float)adcx*(25.2 / 3127);
//		volt *= 10;
        volt = INA226_Read2Byte(WRITE_ADDR_0X85, Bus_V_Reg)*1.25;
		Sensor.Volt = volt;
		Sensor.Speed = Speed;
		if(Sensor.Volt < 22000)
		{
			Sensor.Volt_State = 1;
		}
		else if(Sensor.Volt > 26000)
		{
			Sensor.Volt_State = 2;
		}
		else
		{
			Sensor.Volt_State = 3;
		}
		pSensor = &Sensor;
        
    
//        		if(queue1 != NULL)
//		{
//			xQueueSend(queue1, (void *)&pSensor, (TickType_t)0);
//		}
		vTaskDelayUntil(&PreviousWakeTime, configTICK_RATE_HZ/100);
	}
}
/*发送*/
void Send_Task(void *pvParameters)
{
	TickType_t	PreviousWakeTime = xTaskGetTickCount();
	while(1)
	{
		PreviousWakeTime = xTaskGetTickCount();
//        		if(xQueueReceive(queue1, &(pSensor), ( TickType_t )portMAX_DELAY) == pdPASS)
//		{
//        if(xSemaphoreTake(semaph_msg, (TickType_t)100))
//        {
		//LOG("run the send task\r\n");
//				LOG("%.1f %2d %2d %2d %2d %2d %2d %2d %2d %2d %2d %2d\r\n",
//					pSensor->temp1,pSensor->Temp_Sensor_State,
//					pSensor->PressBuf,pSensor->Light_State,
//					pSensor->PressBuf2,pSensor->Motor_State,
//					pSensor->Volt,pSensor->Speed,pSensor->Volt_State,
//					pSensor->BoardTemp, pSensor->EcsCurrent, pSensor->LightCurrent);			
             if(xSemaphoreTake(semaph_msg, (TickType_t)1000))
            {
                delay_xms(1);
                CAN_Send_Msg(pSensor);
                xSemaphoreGive(semaph_msg);
            }
			//delay_xms(200);
            //xSemaphoreGive(semaph_msg);
//        }
//        else
//        {
//            printf("failed to take sem in send msg\r\n");
//        }
//        }
		vTaskDelayUntil(&PreviousWakeTime, configTICK_RATE_HZ/100);
	}
	
}

/*控制任务*/
void Control_Task(void *pvParameters)
{
	TickType_t	PreviousWakeTime = xTaskGetTickCount();
	uint16_t compare = 1001;
//	uint16_t cold_compare=1001;
	uint16_t My_Compare = 999;
//	uint8_t tep_flag=0;
	//    uint8_t Cold_Flag=0;
	uint8_t CheckSum=0;
	Sensor.Light_State = 2;
	while(1)
	{
		PreviousWakeTime = xTaskGetTickCount();
//		LOG("run the control task\r\n");
//		memset(&Sensor, 0, sizeof(Sensor));
			if(flag == 1)
			{
				CheckSum = TX_SumCheck(RxMessage.Data, 6);
				flag = 0;
				if(RxMessage.Data[0] == 0XCC && RxMessage.Data[7] == 0XDD && RxMessage.Data[6] == CheckSum)
				{
					//电机：0为关闭状态
					if(RxMessage.Data[1] == 0xA1)//急停 
					{
							Close_lighter();
							TIM_SetCompare2(TIM2, 1001);
							Sensor.Light_State = 2;
							Sensor.Motor_State = 2;
                            //Cold_Flag  = 0;
                            My_Compare=1000;
                            compare=1000;

					}
//                        if(RxMessage.Data[1] == 0xA2) //强制冷却
//						{
//                            My_Compare=1100;
//							Cold_Flag = 1;
//                            tep_flag = 0;
//                            cold_compare = (RxMessage.Data[4]<<8) | RxMessage.Data[5];
//        				}
						if(RxMessage.Data[1] == 0xA3) //停车
						{
								My_Compare=1000;
								compare=1000;
								Close_lighter();
								Sensor.Light_State = 2;
								Sensor.Motor_State = 2;
								//Cold_Flag = 1;
//								tep_flag = 0;
								TIM_SetCompare2(TIM2, 1001);
                            
                            //cold_compare = (RxMessage.Data[4]<<8) | RxMessage.Data[5];
        		}
//						if(RxMessage.Data[2] == 0xA2)//解锁电调
//						{
//							Unlock();
//						}
                        
						//点火器：1为启动状态
						if(RxMessage.Data[3] == 0x11)
						{
							Open_lighter();
							Sensor.Light_State = 1;
						}
						//点火器：2为关闭状态
						if(RxMessage.Data[3] == 0x22)
						{
							Close_lighter();
							Sensor.Light_State = 2;
						}
                        
                        if(RxMessage.Data[2] == 0x0E)
                        {
						compare = (RxMessage.Data[4]<<8) | RxMessage.Data[5];
                        }
						//LOG("compare = %X\r\n", compare);
						//电机：1为启动状态
//						if(compare > 0)
//						{
//							if(compare >= 2000)
//							{
//								compare = 2000;
//							}
//							TIM_SetCompare2(TIM2, compare);
//							Sensor.Motor_State = 1;
//						}
//						if(compare < 1000)
//						{
//							compare = 0;
//							TIM_SetCompare2(TIM2, compare);
//							Sensor.Motor_State = 2;
//						}
					}
						
			}
			if(abs(compare - My_Compare) > 0)
			{
				
					if(compare > My_Compare)
					{
//						delay_us(50);
						My_Compare++;
						if(My_Compare > 1700)
						{
							My_Compare =1700;
						}
					}
					else
					{
//						delay_us(50);
						My_Compare--;
						if(My_Compare < 1000)
						{
							My_Compare = 1000;
						}
					}
					TIM_SetCompare2(TIM2, My_Compare);
					if(My_Compare > 1000)
					{
						Sensor.Motor_State = 1;
					}
					else
					{
						Sensor.Motor_State = 2;
					}
//                    if(compare <= 1000)
//                    {
//                        My_Compare = 999;
//                        //My_Compare = 0;
//                        TIM_SetCompare2(TIM2, 0);
//                        Sensor.Motor_State = 2;
//                    }
            }
            //急停
			if(compare <= 1000)
			{
                My_Compare =1001;
				compare = 1001;
				//My_Compare = 0;
				TIM_SetCompare2(TIM2, My_Compare);
				Sensor.Motor_State = 2;
			}
//            if(Cold_Flag == 1)
//            {
////                if(tep_flag == 0)
////                {
////                    tep_flag = 1;
////                    My_Compare= 1001;
////                    TIM_SetCompare2(TIM2, My_Compare);
////                    delay_xms(500);
////                }
////                tep = (uint32_t)Sensor.temp1;
////                if(Sensor.temp1 > 85)
////                {
//////                    if(Speed<cold_compare)
//////                    {
//////                    My_Compare++;
//////                    }
//////                    if(My_Compare > 1500)
//////					{
//////						My_Compare = 1500;
//////					}
////                    My_Compare = 1150;
////                }
////                if(Sensor.temp1 <= 85)
////                {
//////                   delay_xms(3);
//////                   My_Compare--;
//////                   if(My_Compare < 1000)
//////				   {
//////						My_Compare = 1001;
//////                        compare = 1001;
//////                        Cold_Flag = 0;

//////					}
////                        My_Compare = 1001;
////                        compare = 1001;
////                        Cold_Flag = 0;
////                    
////                }
//                TIM_SetCompare2(TIM2, My_Compare);
//            }
		vTaskDelayUntil(&PreviousWakeTime, configTICK_RATE_HZ/1000);
	}
}

/*开始任务任务函数*/
void start_task(void *pvParameters)
{
	taskENTER_CRITICAL();           //进入临界区
	xTaskCreate(LED_Task    ,		"LED_Task"      ,		216 , NULL,	5,	&LED_Task_Handler);
	xTaskCreate(Sensor_Task ,		"Sensor_Task" 	,		256	,	NULL,	5,	&Sensor_Task_Handler);
	xTaskCreate(Send_Task ,			"Send_Task" 		,		256	,	NULL,	5,	&Send_Task_Handler);
	xTaskCreate(Control_Task ,	"Control_Task" 	,		256	,	NULL,	5,	&Control_Task_Handler);
	xTaskCreate(Speed_Send_Task,		"Speed_Send_Task",		216, NULL,	5,	&Speed_Send_Task_Handler);
    
	queue1 = xQueueCreate(3, sizeof(struct SensorDataTypeDef* ));
	if(queue1 == NULL)
	{
		printf("creat queue_send_msg failed!\r\n");
	}
	semaph_msg = xSemaphoreCreateMutex();
	if(semaph_msg == NULL)
	{
		printf("creat semaph_msg failed!\r\n");
	}
	vTaskDelete(NULL);              //删除开始任务
	taskEXIT_CRITICAL();            //退出临界区
}
