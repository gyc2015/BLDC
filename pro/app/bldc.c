#include <bldc.h>
#include <board.h>

void BLDC_Init(struct BLDC *bldc) {
    bldc->cmd.all = 0;
    bldc->duty = 0.0;
    bldc->dir = BLDC_DIR_UNKNOWN;
    bldc->hall = 0;
    bldc->pulse_count = 0;
}

/*
 * BLDC_NOneRotate - 三三导通反转BLDC
 */
void BLDC_NOneRotate(struct BLDC *bldc) {
    // ABC
    uint8 hall = Hall_GetStatus();
    
    PWM_Set_Duty(&PWM_HA, bldc->duty);
    
    HallAout = (hall & 0x04) ? 0 : 1;
    HallBout = (hall & 0x02) ? 0 : 1;
    HallCout = (hall & 0x01) ? 0 : 1;    
}

/*
 * BLDC_OneRotate - 三三导通正转BLDC
 */
void BLDC_OneRotate(struct BLDC *bldc) {
    // ABC
    uint8 hall = Hall_GetStatus();
    
    PWM_Set_Duty(&PWM_HA, bldc->duty);
    
    HallAout = (hall & 0x04) ? 1 : 0;
    HallBout = (hall & 0x02) ? 1 : 0;
    HallCout = (hall & 0x01) ? 1 : 0;
}


/*
 * BLDC_NRotate - 反转BLDC
 */
void BLDC_NRotate(struct BLDC *bldc) {
    // ABC
    uint8 hall = Hall_GetStatus();
    
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
/*
 * BLDC_Rotate - 正转BLDC
 */
void BLDC_Rotate(struct BLDC *bldc) {
    // ABC
    uint8 hall = Hall_GetStatus();
    
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
        // ??????
        break;
    }
}

