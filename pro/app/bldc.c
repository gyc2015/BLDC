#include <bldc.h>

void BLDC_Init(struct BLDC *bldc) {
    bldc->cmd.all = 0;
    bldc->duty = 0.0;
}
/*
 * _BLDC_Remap_Hall - 对霍尔信号的重映射，针对安装时霍尔信号不对，导致换相不正确
 */
static uint8 _BLDC_Remap_Hall(uint8 hall) {
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
 * BLDC_NThreeRotate - 三三导通反转BLDC
 */
void BLDC_NOneRotate(struct BLDC *bldc) {
    // ABC
    uint8 hall = Hall_GetStatus();
    hall = _BLDC_Remap_Hall(hall);
    
    PWM_Set_Duty(&PWM_HA, bldc->duty);
    
    HallAout = (hall & 0x04) ? 0 : 1;
    HallBout = (hall & 0x02) ? 0 : 1;
    HallCout = (hall & 0x01) ? 0 : 1;    
}

/*
 * BLDC_ThreeRotate - 三三导通正转BLDC
 */
void BLDC_OneRotate(struct BLDC *bldc) {
    // ABC
    uint8 hall = Hall_GetStatus();
    hall = _BLDC_Remap_Hall(hall);
    
    PWM_Set_Duty(&PWM_HA, bldc->duty);
    
    HallAout = (hall & 0x04) ? 1 : 0;
    HallBout = (hall & 0x02) ? 1 : 0;
    HallCout = (hall & 0x01) ? 1 : 0;
}

