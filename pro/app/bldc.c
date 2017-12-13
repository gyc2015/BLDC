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
void BLDC_NThreeRotate(struct BLDC *bldc) {
    // ABC
    uint8 hall = Hall_GetStatus();
    hall = _BLDC_Remap_Hall(hall);
    
    PWM_Set_Duty(&PWM_HA, 0);
    PWM_Set_Duty(&PWM_HB, 0);
    PWM_Set_Duty(&PWM_HC, 0);
    
    switch (hall) {
    case 0x5:
        // 101 -> T3, T2, T6, BA_BC
        PWM_Set_Duty(&PWM_HA, 0);
        PWM_Set_Duty(&PWM_HB, bldc->duty);
        PWM_Set_Duty(&PWM_HC, 0);
        break;
    case 0x4:
        // 100 -> T5, T2, T3, CA_BA
        PWM_Set_Duty(&PWM_HA, 0);
        PWM_Set_Duty(&PWM_HB, bldc->duty);
        PWM_Set_Duty(&PWM_HC, bldc->duty);
        break;
    case 0x6:
        // 110 -> T5, T4, T2, CB_CA
        PWM_Set_Duty(&PWM_HA, 0);
        PWM_Set_Duty(&PWM_HB, 0);
        PWM_Set_Duty(&PWM_HC, bldc->duty);
        break;
    case 0x2:
        // 010 -> T1, T4, T5 AB_CB
        PWM_Set_Duty(&PWM_HA, bldc->duty);
        PWM_Set_Duty(&PWM_HB, 0);
        PWM_Set_Duty(&PWM_HC, bldc->duty);
        break;
    case 0x3:
        // 011 -> T1, T6, T4, AC_AB
        PWM_Set_Duty(&PWM_HA, bldc->duty);
        PWM_Set_Duty(&PWM_HB, 0);
        PWM_Set_Duty(&PWM_HC, 0);
        break;
    case 0x1:
        // 001 -> T3, T6, T1, BC_AC
        PWM_Set_Duty(&PWM_HA, bldc->duty);
        PWM_Set_Duty(&PWM_HB, bldc->duty);
        PWM_Set_Duty(&PWM_HC, 0);
        break;
    default:
        // ??????
        break;
    }
}

/*
 * BLDC_ThreeRotate - 三三导通正转BLDC
 */
void BLDC_ThreeRotate(struct BLDC *bldc) {
    // ABC
    uint8 hall = Hall_GetStatus();
    hall = _BLDC_Remap_Hall(hall);
    
    PWM_Set_Duty(&PWM_HA, 0);
    PWM_Set_Duty(&PWM_HB, 0);
    PWM_Set_Duty(&PWM_HC, 0);
    
    switch (hall) {
    case 0x5:
        // 101 -> T1, T5, T4, AB_CB
        PWM_Set_Duty(&PWM_HA, bldc->duty);
        PWM_Set_Duty(&PWM_HB, 0);
        PWM_Set_Duty(&PWM_HC, bldc->duty);
        break;
    case 0x4:
        // 100 -> T1, T4, T6, AC_AB
        PWM_Set_Duty(&PWM_HA, bldc->duty);
        PWM_Set_Duty(&PWM_HB, 0);
        PWM_Set_Duty(&PWM_HC, 0);
        break;
    case 0x6:
        // 110 -> T1, T3, T6, BC_AC
        PWM_Set_Duty(&PWM_HA, bldc->duty);
        PWM_Set_Duty(&PWM_HB, bldc->duty);
        PWM_Set_Duty(&PWM_HC, 0);
        break;
    case 0x2:
        // 010 -> T3, T6, T2, BA_BC
        PWM_Set_Duty(&PWM_HA, 0);
        PWM_Set_Duty(&PWM_HB, bldc->duty);
        PWM_Set_Duty(&PWM_HC, 0);
        break;
    case 0x3:
        // 011 -> T5, T2, T3, CA_BA
        PWM_Set_Duty(&PWM_HA, 0);
        PWM_Set_Duty(&PWM_HB, bldc->duty);
        PWM_Set_Duty(&PWM_HC, bldc->duty);
        break;
    case 0x1:
        // 001 -> T5, T4, T2, CB_CA
        PWM_Set_Duty(&PWM_HA, 0);
        PWM_Set_Duty(&PWM_HB, 0);
        PWM_Set_Duty(&PWM_HC, bldc->duty);
        break;
    default:
        // 不可能的状态
        break;
    }
}

