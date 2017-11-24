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

int main(void) {
    USART1_Init(115200);
    Hall_Init();
    PWM_Init();
    ADC1_Init(adcvalues);
    IO_Init();
    SPI1_Init();
    
    printf("wuhahaha\r\n");

    NVIC_Configuration();
    
    while (1) {
        while (!is_queue_empty(&gU1RxQ)) {
            dequeue(&gU1RxQ, &tmp);
            USART1_SendByte(tmp);
            switch (tmp) {
            case 'a':
                printf("Hall:0x%2x",Hall_GetStatus());
                break;
            case 'A':
                PWM_DeInit();
                break;
            case 'B':
                duty += 0.1;
                printf("H:duty = %f\r\n", duty);
                PWM_Set_Duty(&PWM_HA, duty);
                PWM_Set_Duty(&PWM_HB, duty);
                PWM_Set_Duty(&PWM_HC, duty);
            
                PWM_Set_Duty(&PWM_LA, 0);
                PWM_Set_Duty(&PWM_LB, 0);
                PWM_Set_Duty(&PWM_LC, 0);
                break;
            case 'b':
                duty -= 0.1;
                printf("H:duty = %f\r\n", duty);
                PWM_Set_Duty(&PWM_HA, duty);
                PWM_Set_Duty(&PWM_HB, duty);
                PWM_Set_Duty(&PWM_HC, duty);
            
                PWM_Set_Duty(&PWM_LA, 0);
                PWM_Set_Duty(&PWM_LB, 0);
                PWM_Set_Duty(&PWM_LC, 0);
                break;
            case 'C':
                duty += 0.1;
                printf("L:duty = %f\r\n", duty);
                PWM_Set_Duty(&PWM_LA, duty);
                PWM_Set_Duty(&PWM_LB, duty);
                PWM_Set_Duty(&PWM_LC, duty);
            
                PWM_Set_Duty(&PWM_HA, 0);
                PWM_Set_Duty(&PWM_HB, 0);
                PWM_Set_Duty(&PWM_HC, 0);
                break;
            case 'c':
                duty -= 0.1;
                printf("L:duty = %f\r\n", duty);
                PWM_Set_Duty(&PWM_LA, duty);
                PWM_Set_Duty(&PWM_LB, duty);
                PWM_Set_Duty(&PWM_LC, duty);
            
                PWM_Set_Duty(&PWM_HA, 0);
                PWM_Set_Duty(&PWM_HB, 0);
                PWM_Set_Duty(&PWM_HC, 0);
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
                SPI1_Communicate(0, 7, 0x80);
                SPI1_Communicate(0, 0x0C, 0x3C8);
                printf("0x05 HS Gate Driver = %x\r\n", SPI1_Communicate(1, 5, 0));
                printf("0x06 LS Gate Driver = %x\r\n", SPI1_Communicate(1, 6, 0));
                printf("0x07 Gate Driver Control = %x\r\n", SPI1_Communicate(1, 7, 0));
                printf("0x0C VDS Sense Control = %x\r\n", SPI1_Communicate(1, 0x0C, 0));
                break;
            }
        }
    }
}

