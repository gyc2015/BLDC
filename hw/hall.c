#include <board.h>

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
void Hall_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    PBout(11) = 0;
    
    _Hall_Exti_Init();
}
/*
 * Hall_GetStatus - 获取霍尔状态
 */
uint8 Hall_GetStatus(void) {
    return ((GPIO_ReadInputData(GPIOB) >> 12) & 0x07);
}

#define HallExti_ARising 0
#define HallExti_AFaling 1
#define HallExti_BRising 2
#define HallExti_BFaling 3
#define HallExti_CRising 4
#define HallExti_CFaling 5
#define HallExti_Unknown 6

uint8 gCurExt = HallExti_Unknown;
uint8 gNcurExt = HallExti_Unknown;
uint8 gLstExt = HallExti_Unknown;
uint8 gPosExt = HallExti_Unknown;
uint8 gNegExt = HallExti_Unknown;

int32 gCount = 0;

void EXTI15_10_IRQHandler(void) {
    if (EXTI_GetITStatus(EXTI_Line14) != RESET) {
        // PB14 - HallA
        if (1 == HallA) {
            gCurExt = HallExti_ARising;
            if (gCurExt == gPosExt) {
                gCount++;
            } else if (gCurExt == gNegExt) {
                gCount--;
            }
            gNcurExt = HallExti_AFaling;
            gPosExt = HallExti_CFaling;
            gNegExt = HallExti_BFaling;
        } else if (0 == HallA) {
            gCurExt = HallExti_AFaling;
            if (gCurExt == gPosExt) {
                gCount++;
            } else if (gCurExt == gNegExt) {
                gCount--;
            }
            gNcurExt = HallExti_ARising;
            gPosExt = HallExti_CRising;
            gNegExt = HallExti_BRising;
        }
        EXTI_ClearITPendingBit(EXTI_Line14);
    }
    if (EXTI_GetITStatus(EXTI_Line13) != RESET) {
        // PB13 - HallB
        if (1 == HallB) {
            gCurExt = HallExti_BRising;
            if (gCurExt == gPosExt) {
                gCount++;
            } else if (gCurExt == gNegExt) {
                gCount--;
            }
            gNcurExt = HallExti_BFaling;
            gPosExt = HallExti_AFaling;
            gNegExt = HallExti_CFaling;
        } else if (0 == HallB) {
            gCurExt = HallExti_BFaling;
            if (gCurExt == gPosExt) {
                gCount++;
            } else if (gCurExt == gNegExt) {
                gCount--;
            }
            gNcurExt = HallExti_BRising;
            gPosExt = HallExti_ARising;
            gNegExt = HallExti_CRising;
        }
        EXTI_ClearITPendingBit(EXTI_Line13);
    }
    if (EXTI_GetITStatus(EXTI_Line12) != RESET) {
        // PB12 - HallC
        if (1 == HallC) {
            gCurExt = HallExti_CRising;
            if (gCurExt == gPosExt) {
                gCount++;
            } else if (gCurExt == gNegExt) {
                gCount--;
            }
            gNcurExt = HallExti_CFaling;
            gPosExt = HallExti_BFaling;
            gNegExt = HallExti_AFaling;
        } else if (0 == HallC) {
            gCurExt = HallExti_CFaling;
            if (gCurExt == gPosExt) {
                gCount++;
            } else if (gCurExt == gNegExt) {
                gCount--;
            }
            gNcurExt = HallExti_CRising;
            gPosExt = HallExti_BRising;
            gNegExt = HallExti_ARising;
        }
        EXTI_ClearITPendingBit(EXTI_Line12);
    }

}

