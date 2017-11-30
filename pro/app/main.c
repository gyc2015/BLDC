#include <board.h>
#include <command.h>
#include <drv8305.h>
#include <bldc.h>
/*
 * NVIC_Configuration - 中断优先级配置
 */
void NVIC_Configuration(void){	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	// USART1 串口中断
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    // EXTI 外部中断
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


int gSCount = 0;

struct Drv8305 gDrv8305;
struct BLDC gBldc;

uint8 remap_hall(uint8 hall) {
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

void nrotate_motor(struct BLDC *bldc) {
    // ABC
    uint8 hall = Hall_GetStatus();
    hall = remap_hall(hall);
    
    PWM_Set_Duty(&PWM_HA, 0);
    PWM_Set_Duty(&PWM_HB, 0);
    PWM_Set_Duty(&PWM_HC, 0);
    PWM_Set_Duty(&PWM_LA, 0);
    PWM_Set_Duty(&PWM_LB, 0);
    PWM_Set_Duty(&PWM_LC, 0);
    
    switch (hall) {
    case 0x5:
        // 101 -> T3, T2
        PWM_Set_Duty(&PWM_HB, bldc->duty);
        PWM_Set_Duty(&PWM_LA, 1);
        break;
    case 0x4:
        // 100 -> T5, T2
        PWM_Set_Duty(&PWM_HC, bldc->duty);
        PWM_Set_Duty(&PWM_LA, 1);
        break;
    case 0x6:
        // 110 -> T5, T4
        PWM_Set_Duty(&PWM_HC, bldc->duty);
        PWM_Set_Duty(&PWM_LB, 1);
        break;
    case 0x2:
        // 010 -> T1, T4
        PWM_Set_Duty(&PWM_HA, bldc->duty);
        PWM_Set_Duty(&PWM_LB, 1);
        break;
    case 0x3:
        // 011 -> T1, T6
        PWM_Set_Duty(&PWM_HA, bldc->duty);
        PWM_Set_Duty(&PWM_LC, 1);
        break;
    case 0x1:
        // 001 -> T3, T6
        PWM_Set_Duty(&PWM_HB, bldc->duty);
        PWM_Set_Duty(&PWM_LC, 1);
        break;
    default:
        // ??????
        break;
    }
}



void rotate_motor(struct BLDC *bldc) {
    // ABC
    uint8 hall = Hall_GetStatus();
    hall = remap_hall(hall);
    
    PWM_Set_Duty(&PWM_HA, 0);
    PWM_Set_Duty(&PWM_HB, 0);
    PWM_Set_Duty(&PWM_HC, 0);
    PWM_Set_Duty(&PWM_LA, 0);
    PWM_Set_Duty(&PWM_LB, 0);
    PWM_Set_Duty(&PWM_LC, 0);
    
    switch (hall) {
    case 0x5:
        // 101 -> T1, T4
        PWM_Set_Duty(&PWM_HA, bldc->duty);
        PWM_Set_Duty(&PWM_LB, 1);
        break;
    case 0x4:
        // 100 -> T1, T6
        PWM_Set_Duty(&PWM_HA, bldc->duty);
        PWM_Set_Duty(&PWM_LC, 1);
        break;
    case 0x6:
        // 110 -> T3, T6
        PWM_Set_Duty(&PWM_HB, bldc->duty);
        PWM_Set_Duty(&PWM_LC, 1);
        break;
    case 0x2:
        // 010 -> T3, T2
        PWM_Set_Duty(&PWM_HB, bldc->duty);
        PWM_Set_Duty(&PWM_LA, 1);
        break;
    case 0x3:
        // 011 -> T5, T2
        PWM_Set_Duty(&PWM_HC, bldc->duty);
        PWM_Set_Duty(&PWM_LA, 1);
        break;
    case 0x1:
        // 001 -> T5, T4
        PWM_Set_Duty(&PWM_HC, bldc->duty);
        PWM_Set_Duty(&PWM_LB, 1);
        break;
    default:
        // 不可能的状态
        break;
    }
}

BOOL CheckAddress(uint16 add) {
    uint8 page = (uint8)((add & 0xFF00) >> 8);    

    switch (page) {
    case CFG_REGPAGE_COMMAND: return TRUE;
    case CFG_REGPAGE_DRV8305: return TRUE;
    }
    
    return FALSE;
}

void SetRegister(uint16 addr, uint8 data) {
    uint8 page = (uint8)((addr & 0xFF00) >> 8);
    uint8 reg = (uint8)(addr & 0xFF);
    uint8 *ptr = NULL;
    
    if (CFG_REGPAGE_COMMAND == page) {
        if (reg < sizeof(gBldc)) {
            ptr = (uint8*)&gBldc;
            ptr[reg] = data;
        }
    } else if (CFG_REGPAGE_DRV8305 == page) {
        if (reg < sizeof(gDrv8305)) {
            ptr = (uint8*)&gDrv8305;
            ptr[reg] = data;
        }
        
        if (0 == reg || 1 == reg)
            DRV_Exec(&gDrv8305);
    }
}

uint8 GetRegister(uint16 addr) {
    uint8 page = (uint8)((addr & 0xFF00) >> 8);
    uint8 reg = (uint8)(addr & 0xFF);
    uint8 *ptr = NULL;
    
    if (CFG_REGPAGE_COMMAND == page) {
        if (reg < sizeof(gBldc)) {
            ptr = (uint8*)&gBldc;
            return ptr[reg];
        }
    } else if (CFG_REGPAGE_DRV8305 == page) {
        if (reg < sizeof(gDrv8305)) {
            ptr = (uint8*)&gDrv8305;
            return ptr[reg];
        }
    }
    return 0;
}

int main(void) {
    Init_Delay(72);
    USART1_Init(115200);
    Hall_Init();
    PWM_Init();
    ADC1_Init((uint16*)&(gBldc.vi));
    IO_Init();
    SPI1_Init();

    Cmd_Init(&gU1RxQ, CheckAddress, SetRegister, GetRegister);
    NVIC_Configuration();
    
    Delay_ms(100);
    
    BLDC_Init(&gBldc);
    DRV_Init(&gDrv8305);
    EN_GATE = 0;
    printf("wuhahaha\r\n");

    while (1) {
        EN_GATE = gBldc.cmd.bits.en;
        if (gBldc.cmd.bits.en) {
            if (gBldc.cmd.bits.dir)
                rotate_motor(&gBldc);
            else
                nrotate_motor(&gBldc);            
        }
        Cmd_Parse();
    }
}
