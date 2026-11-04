#include "led.h"
#include "delay.h"
#include "sys.h"
#include "FreeRTOS.h"
#include "task.h"

#include "iwdg.h"
#include "Pressure.h"
#include "adc.h"
#include "timer.h"
#include "can.h"
#include "SPI.h"
#include "Temp.h"
#include "lighter.h"
#include "M1820B.h"
#include "bsp_ina226.h"
//#include "IIC.h"
//#include "HP5834_BA.h"
#include "Pressure_BA.h"
RCC_ClocksTypeDef RCC_CLK;/*  时钟结构体定义  */
extern void Freertos_Init(void);
int main(void)
{
  /* Infinite loop */
	/*Total RO  Size ：表示程序占用的Flash空间大小*/
	/*Total RW  Size ：表示程序运行时占用的RAM空间大小*/
	/*Total ROM Size ：表示程序下载到Flash占用空间大小*/
	delay_init();																	/*延时初始化*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); /*设置系统中断优先级分组4*/
	RCC_GetClocksFreq(&RCC_CLK);                    /*获取系统时钟*/
	if(RCC_CLK.SYSCLK_Frequency != 72000000)
	{
		while(1);/*如果系统时钟不正确，则不运行后面的程序*/
	}
	//将PB3和PB4作为普通IO的重映射
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE );
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 
	uart_init(115200);
	LED_Init(); 												/*LED初始化*/
	HP5834_Init();											/*气压1初始化*/
	//HP5834_BA_Init();										/*气压2的初始化*/
	HP5835_BA_Init();
	Lighter_Init();
	AD_Init();													/*ADC初始化*/
	Encoder_Init();											/*编码器初始化*/
	Timer_Init();												/*定时器初始化*/
	CAN1_Init();												/*CAN总线初始化*/
	MySPI_Init();												/*SPI初始化*/
	TIM2_PWM_Init();										/*PWM输出初始化*/
	M1820B_GPIO_Init();
    INA226_Init(WRITE_ADDR_0X85);
    INA226_Init(WRITE_ADDR);
	//IWDG_Init(64, 5000);
	Freertos_Init();

	/*此函数只在电调第一次上电启动时需要调用*/
	//Unlock();														/*电调解锁*/
	
	
  vTaskStartScheduler();             //开启任务调度
	
  while (1)
  {
		
  }
}



