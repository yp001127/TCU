#ifndef __ENCODER_H
#define __ENCODER_H

#include "sys.h"
extern uint16_t Speed;
void Encoder_Init(void);
int16_t Encoder_Get(void);
void Timer_Init(void);
void TIM2_PWM_Init(void);
void Unlock(void);
#endif
