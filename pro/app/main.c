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
    switch (hall) {
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
        // ??????
        break;
    }
}


int main(void) {
    Init_Delay(72);
    USART1_Init(115200);
    Hall_Init();
    PWM_Init();
    ADC1_Init(adcvalues);
    IO_Init();
    SPI1_Init();
    
    printf("wuhahaha\r\n");

    NVIC_Configuration();
    
    Delay_ms(100);
    SPI1_Communicate(0, 9, 0x322);
    while (1) {
        if (TRUE == gDir)
            rotate_motor();
        else
            nrotate_motor();
        
        if (!nFAULT) {
            //SPI1_Communicate(0, 9, 0x322);
        }
        
        while (!is_queue_empty(&gU1RxQ)) {
            dequeue(&gU1RxQ, &tmp);
            USART1_SendByte(tmp);
            switch (tmp) {
            case 'a':
                printf("Hall:0x%2x",Hall_GetStatus());
                if (1 == nFAULT)
                    printf("DRV8305正常");
                else
                    printf("DRV8305出错");
                break;
            case 'A':
                PWM_DeInit();
                break;
            case 'D':
                printf("PVDDSENSE = %fV\r\n", 3.3 * adcvalues[0] / 4095 * 54.8 / 20);
                printf("CSENSE = %fV\r\n", 3.3 * adcvalues[1] / 4095 * 54.8 / 20);
                printf("BSENSE = %fV\r\n", 3.3 * adcvalues[2] / 4095 * 54.8 / 20);
                printf("ASENSE = %fV\r\n", 3.3 * adcvalues[3] / 4095 * 54.8 / 20);
                printf("SO1 = %d\r\n", adcvalues[4]);
                printf("SO2 = %d\r\n", adcvalues[5]);
                printf("SO3 = %d\r\n", adcvalues[6]);
                break;
            case 'd':
                printf("0x01 Warning and Watchdog = %x\r\n", SPI1_Communicate(1, 1, 0));
                printf("0x02 OV/VDS Faults = %x\r\n", SPI1_Communicate(1, 2, 0));
                printf("0x03 IC Faults = %x\r\n", SPI1_Communicate(1, 3, 0));
                printf("0x04 Gate Driver VGS Faults = %x\r\n", SPI1_Communicate(1, 4, 0));        
                printf("0x05 HS Gate Driver = %x\r\n", SPI1_Communicate(1, 5, 0));
                printf("0x06 LS Gate Driver = %x\r\n", SPI1_Communicate(1, 6, 0));
                printf("0x07 Gate Driver Control = %x\r\n", SPI1_Communicate(1, 7, 0));
                printf("0x09 IC Operation = %x\r\n", SPI1_Communicate(1, 9, 0));
                printf("0x0A Shunt Amplifier Control = %x\r\n", SPI1_Communicate(1, 0xA, 0));
                printf("0x0B Voltage Regulator Control = %x\r\n", SPI1_Communicate(1, 0x0B, 0));            
                printf("0x0C VDS Sense Control = %x\r\n", SPI1_Communicate(1, 0x0C, 0));
                break;
            case 'E':
                SPI1_Communicate(0, 5, 0x3BB);
                SPI1_Communicate(0, 6, 0x3BB);
                printf("EN_GATE = 1\r\n");
                EN_GATE = 1;
                break;
            case 'e':
                printf("EN_GATE = 0\r\n");
                EN_GATE = 0;
                break;
            case 'f':
                SPI1_Communicate(0, 5, 0x344);
                SPI1_Communicate(0, 6, 0x344);
                SPI1_Communicate(0, 7, 0x216);
                SPI1_Communicate(0, 0x0C, 0x2C8);
                printf("0x05 HS Gate Driver = %x\r\n", SPI1_Communicate(1, 5, 0));
                printf("0x06 LS Gate Driver = %x\r\n", SPI1_Communicate(1, 6, 0));
                printf("0x07 Gate Driver Control = %x\r\n", SPI1_Communicate(1, 7, 0));
                printf("0x0C VDS Sense Control = %x\r\n", SPI1_Communicate(1, 0x0C, 0));
                break;
            case 'g':
                duty += 0.01;
                printf("H:duty = %f\r\n", duty);
                gRotate = TRUE;
                gDir = TRUE;
                break;
            case 'G':
                duty -= 0.01;
                printf("H:duty = %f\r\n", duty);
                gRotate = TRUE;
                gDir = TRUE;
                break;
            case 'h':
                duty += 0.1;
                printf("H:duty = %f\r\n", duty);
                gRotate = TRUE;
                gDir = TRUE;
                break;
            case 'H':
                duty -= 0.1;
                printf("H:duty = %f\r\n", duty);
                gRotate = TRUE;
                gDir = TRUE;
                break;
            case 'i':
                duty = 0.5;
                printf("H:duty = %f\r\n", duty);
                gRotate = TRUE;
                gDir = TRUE;
                //rotate_motor();
                break;
            case 'I':
                duty = 0.55;
                printf("H:duty = %f\r\n", duty);
                gRotate = TRUE;
                gDir = FALSE;
                //nrotate_motor();
                break;
            }
        }
    }
}
