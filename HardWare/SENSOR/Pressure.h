#ifndef __PRESSURE_H
#define __PRESSURE_H

#include "sys.h"

#define HP5834_RST_ADDR				0x06			/*软件复位模块*/
#define HP5834_READ_PT_ADDR 	0x10			/*读温度和压力*/
#define HP5834_READ_P_ADDR 		0x30			/*只读压力命令*/
#define HP5834_READ_A_ADDR 		0x31			/*只读海拔命令*/
#define HP5834_READ_T_ADDR 		0x32			/*只读温度命令*/
#define HP5834_ANA_CAL_ADDR 	0x28			/*重新校准内部模块*/
#define HP5834_WRITE_ADDR			0xEC			/*模块地址，写*/
#define HP5834_READ_ADDR			0xED			/*模块地址，读*/

void HP5834_WriteReg(uint8_t addr, uint8_t data);
uint8_t Read_Byte(uint8_t addr);
void HP5834_Init(void);
void ReadPressure(int32_t* PressBuf);
#endif

