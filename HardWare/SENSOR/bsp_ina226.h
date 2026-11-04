/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_INA226_H__
#define __BSP_INA226_H__

#ifdef __cplusplus
extern "C" {
#endif


/////



#define READ_ADDR                  0x81	 //A0=GND£¬A1=GND // R=1, W=0
#define WRITE_ADDR                 0x80

//#define READ_ADDR_0X85							0x86	 //A0=3V3£¬A1=3V3 // R=1, W=0
#define WRITE_ADDR_0X85							0x8a

#define Config_Reg                 0x00
#define Shunt_V_Reg                0x01
#define Bus_V_Reg                  0x02
#define Power_Reg                  0x03
#define Current_Reg                0x04
#define Calib_Reg                  0x05
#define Mask_En_Reg                0x06
#define Alert_Reg                  0x07
#define Man_ID_Reg                 0xFE  //0x5449
#define ID_Reg                     0xFF  //0x2260

/* Includes ------------------------------------------------------------------*/
#include "bsp_i2c.h"
#include "sys.h"
#include "stdio.h"


uint16_t INA226_Read2Byte(uint16_t cmd, uint8_t reg_addr);
uint8_t INA226_WriteByte(uint16_t cmd, uint8_t reg_addr,uint16_t reg_data);
void INA226_Init(uint16_t Cmd);


/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__BSP_INA226_H__ */

