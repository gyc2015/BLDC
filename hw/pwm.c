/**********************************************
 * ����6ͨ��PWM�������
 *
 * A������ͨ���ֱ����PB10,PB11��Ҫ��ӳ��
 *********************************************/
#include <board.h>

/*
 * ϵͳƵ�� 72MHz PWMƵ�� 72KHz
 */
#define PRESCALER 1
#define PERIOD 1000

/*
 * PWMC_Init - ��ʼ��C������PWM
 *
 * PB10->TIM2CH3->INLC
 * PB11->TIM2CH4->INHC
 */
void PWMC_Init(void) {
    // ����ʱ�� �˿���ӳ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
    GPIO_PinRemapConfig(GPIO_PartialRemap2_TIM2,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    // �˿�����
    GPIO_InitTypeDef init;
    init.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    init.GPIO_Mode = GPIO_Mode_AF_PP;
    init.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &init);
    // TIM2ʱ����Ԫ����
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = PERIOD - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = PRESCALER - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    // CH3,CH4�������
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);
    
    TIM_ARRPreloadConfig(TIM2, ENABLE);
	TIM_Cmd(TIM2, DISABLE);
}

/*
 * PWMAB_Init - ��ʼ��AB������PWM
 *
 * PA8->TIM1CH1->INLB
 * PA9->TIM1CH2->INHB
 * PA10->TIM1CH3->INLA
 * PA11->TIM1CH4->INHA
 */
void PWMAB_Init(void) {
    // ����ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    // �˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    // TIM1ʱ����Ԫ����
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = PERIOD - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = PRESCALER - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    // CH1,CH2,CH3,CH4�������
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_Pulse = 0;
    //
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);
    TIM_OC4Init(TIM1, &TIM_OCInitStructure);
    
    TIM_ARRPreloadConfig(TIM1, ENABLE);
	TIM_Cmd(TIM1, DISABLE);
}
/*
 * PWM_Init - ��ʼ��PWM
 */
void PWM_Init(void) {
    PWMC_Init();
    PWMAB_Init();
    
    TIM_Cmd(TIM1, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
    
    PWM_Set_Duty(&PWM_HA, 0);
    PWM_Set_Duty(&PWM_LA, 0);
    PWM_Set_Duty(&PWM_HB, 0);
    PWM_Set_Duty(&PWM_LB, 0);
    PWM_Set_Duty(&PWM_HC, 0);
    PWM_Set_Duty(&PWM_LC, 0);
    
}
/*
 * PWM_DeInit - �ر�PWM���
 */
void PWM_DeInit(void) {
    TIM_Cmd(TIM1, DISABLE);
    TIM_Cmd(TIM2, DISABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/*
 * PWM_Set_Duty - �趨PWM���ռ�ձ�
 */
void PWM_Set_Duty(volatile uint16 *channel, double n) {
    *channel = PERIOD * n;
}



