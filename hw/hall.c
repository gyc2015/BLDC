#include <board.h>
#include <system.h>

struct BLDC *_gBldcPtr = NULL;

static void _Hall_Exti_Init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); 
    // GPIO的外部中断映射
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource13);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
    // 中断线初始化
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_Line = EXTI_Line12 | EXTI_Line13 | EXTI_Line14;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}

/*
 * Init_Hall - 初始化电机三相霍尔
 */
void Hall_Init(struct BLDC *pbldc) {
    GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    PBout(11) = 0;
    
    _gBldcPtr = pbldc;
    _Hall_Exti_Init();
}

/*
 * _BLDC_Remap_Hall - 对霍尔信号的重映射，针对安装时霍尔信号不对，导致换相不正确
 */
static uint8 _Hall_Remap(uint8 hall) {
    switch (hall) { // CAB
    case 0x5: return 0x6;  // 101(ACB) -> 110(ABC)
    case 0x4: return 0x4;  // 100(ACB) -> 100(ABC)
    case 0x6: return 0x5;  // 110(ACB) -> 101(ABC)
    case 0x2: return 0x1;  // 010(ACB) -> 001(ABC)
    case 0x3: return 0x3;  // 011(ACB) -> 011(ABC)
    case 0x1: return 0x2;  // 001(ACB) -> 010(ABC)
    default: return 0x0;
    }
}

/*
 * Hall_GetStatus - 获取霍尔状态
 */
uint8 Hall_GetStatus(void) {
    return _Hall_Remap((GPIO_ReadInputData(GPIOB) >> 12) & 0x07);
}

uint8 _gNegNext[7] = { 0, 5, 3, 1, 6, 4, 2};
uint8 _gPosNext[7] = { 0, 3, 6, 2, 5, 1, 4};
struct systicks _gLastTicks = {0, 0};
/*
 * _Hall_CountPulse - 计数霍尔脉冲
 */
static void _Hall_CountPulse(void) {
    uint8 hall = Hall_GetStatus();
    if (0 == hall)
        return;
    
    if (0 == _gBldcPtr->hall) {
        _gBldcPtr->hall = hall;
        return;
    }

    struct systicks cticks;
    sys_get_ticks(&cticks);
    int ticks = sys_diff_ticks(&_gLastTicks, &cticks);
    
    if (hall == _gPosNext[_gBldcPtr->hall]) {
        _gBldcPtr->hall = hall;
        _gBldcPtr->dir = BLDC_DIR_POS;
        _gBldcPtr->pulse_count++;
        _gBldcPtr->pulse_rate = 1.0f * (float)CFG_SYSTICK_PMS / (float)ticks;
    } else if (hall == _gNegNext[_gBldcPtr->hall]) {
        _gBldcPtr->hall = hall;
        _gBldcPtr->dir = BLDC_DIR_NEG;
        _gBldcPtr->pulse_count--;
        _gBldcPtr->pulse_rate = -1.0f * (float)CFG_SYSTICK_PMS / (float)ticks;
    }
    _gLastTicks.ms = cticks.ms;
    _gLastTicks.ticks = cticks.ticks;
}

// 因为ACB接线,所以BC反相
void EXTI15_10_IRQHandler(void) {
    if (EXTI_GetITStatus(EXTI_Line14) != RESET) {
        // PB14 - HallA
        _Hall_CountPulse();
        EXTI_ClearITPendingBit(EXTI_Line14);
    }
    if (EXTI_GetITStatus(EXTI_Line12) != RESET) {
        // PB13 - HallB
        _Hall_CountPulse();
        EXTI_ClearITPendingBit(EXTI_Line12);
    }
    if (EXTI_GetITStatus(EXTI_Line13) != RESET) {
        // PB12 - HallC
        _Hall_CountPulse();
        EXTI_ClearITPendingBit(EXTI_Line13);
    }

}

