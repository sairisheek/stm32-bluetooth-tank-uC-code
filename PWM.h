#ifndef __STM32L476G_DISCOVERY_LED_H
#define __STM32L476G_DISCOVERY_LED_H

#include "stm32l476xx.h"

void PWM_Init(void);
void PWM_forward(void);
void PWM_reverse(void);
void PWM_left(void);
void PWM_right(void);
void PWM_stop(void);
#endif
