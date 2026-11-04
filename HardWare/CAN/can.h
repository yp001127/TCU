#ifndef __CAN_H
#define __CAN_H

#include "sys.h"
#include "Process_DT.h"
#include "string.h"
#include "stdio.h"
void CAN1_Init(void);
//void CAN_SetMsg(CanTxMsg *TxMessage);
////uint8_t CAN_GetMsg(uint8_t *msg1, uint8_t *msg2);
//void Init_RxMes(CanRxMsg *RxMessage);
uint8_t CAN_Send_Msg(const SensorDataTypeDef* msg);
uint8_t Can_Receive_Msg(uint8_t* buf);



#endif

