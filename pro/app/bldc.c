#include <bldc.h>

void BLDC_Init(struct BLDC *bldc) {
    bldc->cmd.all = 0;
    bldc->duty = 0.0;
}

