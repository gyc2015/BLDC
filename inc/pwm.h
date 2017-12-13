#ifndef PWM_H
#define PWM_H

#include <types.h>

#define PWM_HA  (TIM1->CCR4)

void PWM_Init(void);
void PWM_DeInit(void);
void PWM_Set_Duty(volatile uint16 *channel, float n);


#endif
