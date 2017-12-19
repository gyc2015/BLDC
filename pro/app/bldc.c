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
 * BLDC_NThreeRotate - 三三导通反转BLDC
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
 * BLDC_ThreeRotate - 三三导通正转BLDC
 */
void BLDC_OneRotate(struct BLDC *bldc) {
    // ABC
    uint8 hall = Hall_GetStatus();
    
    PWM_Set_Duty(&PWM_HA, bldc->duty);
    
    HallAout = (hall & 0x04) ? 1 : 0;
    HallBout = (hall & 0x02) ? 1 : 0;
    HallCout = (hall & 0x01) ? 1 : 0;
}

