#include <board.h>

/*
 * NVIC_Configuration - 中断优先级配置
 */
void NVIC_Configuration(void){	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	// USART1 中断配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

uint8 tmp = 0;
double duty = 0.0;
uint16 adcvalues[7];
bool gRotate = FALSE;
bool gDir = TRUE;

uint8 remap_hall(uint8 hall) {
    switch (hall) { // CAB
    case 0x5: return 0x6; // 101(ACB) -> 110(ABC)
    case 0x4: return 0x4; // 100(ACB) -> 100(ABC)
    case 0x6: return 0x5; // 110(ACB) -> 101(ABC)
    case 0x2: return 0x01; // 010(ACB) -> 001(ABC)
    case 0x3: return 0x3; // 011 -> 011
    case 0x1: return 0x2;  // 001 -> 010
    default: return 0x0;
    }
}

void nrotate_motor() {
    // ABC
    uint8 hall = Hall_GetStatus();
    hall = remap_hall(hall);
    
    PWM_Set_Duty(&PWM_HA, 0);
    PWM_Set_Duty(&PWM_HB, 0);
    PWM_Set_Duty(&PWM_HC, 0);
    PWM_Set_Duty(&PWM_LA, 0);
    PWM_Set_Duty(&PWM_LB, 0);
    PWM_Set_Duty(&PWM_LC, 0);
    
    if (FALSE == gRotate)
        return;
    
    switch (hall) {
    case 0x5:
        // 101 -> T3, T2
        PWM_Set_Duty(&PWM_HB, duty);
        PWM_Set_Duty(&PWM_LA, 1);
        break;
    case 0x4:
        // 100 -> T5, T2
        PWM_Set_Duty(&PWM_HC, duty);
        PWM_Set_Duty(&PWM_LA, 1);
        break;
    case 0x6:
        // 110 -> T5, T4
        PWM_Set_Duty(&PWM_HC, duty);
        PWM_Set_Duty(&PWM_LB, 1);
        break;
    case 0x2:
        // 010 -> T1, T4
        PWM_Set_Duty(&PWM_HA, duty);
        PWM_Set_Duty(&PWM_LB, 1);
        break;
    case 0x3:
        // 011 -> T1, T6
        PWM_Set_Duty(&PWM_HA, duty);
        PWM_Set_Duty(&PWM_LC, 1);
        break;
    case 0x1:
        // 001 -> T3, T6
        PWM_Set_Duty(&PWM_HB, duty);
        PWM_Set_Duty(&PWM_LC, 1);
        break;
    default:
        // 不可能的状态
        break;
    }
}

void rotate_motor() {
    // ABC
    uint8 hall = Hall_GetStatus();
    hall = remap_hall(hall);
    
    PWM_Set_Duty(&PWM_HA, 0);
    PWM_Set_Duty(&PWM_HB, 0);
    PWM_Set_Duty(&PWM_HC, 0);
    PWM_Set_Duty(&PWM_LA, 0);
    PWM_Set_Duty(&PWM_LB, 0);
    PWM_Set_Duty(&PWM_LC, 0);
    
    if (FALSE == gRotate)
        return;
    
    switch (hall) {
    case 0x5:
        // 101 -> T1, T4
        PWM_Set_Duty(&PWM_HA, duty);
        PWM_Set_Duty(&PWM_LB, 1);
        break;
    case 0x4:
        // 100 -> T1, T6
        PWM_Set_Duty(&PWM_HA, duty);
        PWM_Set_Duty(&PWM_LC, 1);
        break;
    case 0x6:
        // 110 -> T3, T6
        PWM_Set_Duty(&PWM_HB, duty);
        PWM_Set_Duty(&PWM_LC, 1);
        break;
    case 0x2:
        // 010 -> T3, T2
        PWM_Set_Duty(&PWM_HB, duty);
        PWM_Set_Duty(&PWM_LA, 1);
        break;
    case 0x3:
        // 011 -> T5, T2
        PWM_Set_Duty(&PWM_HC, duty);
        PWM_Set_Duty(&PWM_LA, 1);
        break;
    case 0x1:
        // 001 -> T5, T4
        PWM_Set_Duty(&PWM_HC, duty);
        PWM_Set_Duty(&PWM_LB, 1);
        break;
    default:
        // 不可能的状态
        break;
    }
}

uint8 gState = 3;
uint8 gCState = 3;
int gSCount = 0;

int main(void) {
    Init_Delay(72);
    USART1_GPIO_Init();
    Hall_Init();
    PWM_Init();
    ADC1_Init(adcvalues);
    IO_Init();
    SPI1_Init();

    //NVIC_Configuration();
    
    Delay_ms(100);
    SPI1_Communicate(0, 5, 0x3BB);
    SPI1_Communicate(0, 6, 0x3BB);
    duty = 0.4;
    gRotate = TRUE;
    EN_GATE = 1;
    
    while (1) {
        if ((0 == USART1_KEY_1) && (0 != USART1_KEY_2)) {
            gCState = 1;
        }
        else if ((0 == USART1_KEY_2) && (0 != USART1_KEY_1)) {
            gCState = 2;                
        }
        else {
            gCState = 3;
        }
        
        if (gCState != gState) {
            gSCount = 0;
            EN_GATE = 0;
        } else {
            gSCount++;
        }
        gState = gCState;

        if (gSCount > 10000) {
            EN_GATE = 1;
            gSCount = 10000;
            if (1 == gCState)
                rotate_motor();
            else if (2 == gCState)
                nrotate_motor(); 
        }
    }
}

