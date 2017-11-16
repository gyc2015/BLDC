#ifndef PWM_H
#define PWM_H

#include <types.h>

#define PWM_HA  (TIM2->CCR3)
#define PWM_LA  (TIM2->CCR4)
#define PWM_HB  (TIM1->CCR1)
#define PWM_LB  (TIM1->CCR2)
#define PWM_HC  (TIM1->CCR3)
#define PWM_LC  (TIM1->CCR4)

void PWM_Init(void);
void PWM_DeInit(void);
void PWM_Set_Duty(volatile uint16 *channel, double n);


#endif
