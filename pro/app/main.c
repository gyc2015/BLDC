#include <board.h>
#include <command.h>
#include <drv8305.h>
#include <bldc.h>
#include <system.h>
#include <xtos.h>

int gSCount = 0;

struct Drv8305 gDrv8305;
struct BLDC gBldc;


/*
 * NVIC_Configuration - 中断优先级配置
 */
void NVIC_Configuration(void){	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	// USART1 串口中断
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
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


#include <cmd_task.h>

#define TASKA_STK_SIZE 512
#define TASKB_STK_SIZE 128

static uint32 taskA_Stk[TASKA_STK_SIZE];
static uint32 taskB_Stk[TASKB_STK_SIZE];

static struct xtos_task_descriptor taskA;
static struct xtos_task_descriptor taskB;

void taskb() {
    gBldc.duty = 0.1;
    gBldc.cmd.bits.en = 1;
    while (1) {
        EN_GATE = gBldc.cmd.bits.en;
        if (gBldc.cmd.bits.en) {
            if (gBldc.cmd.bits.dir)
                BLDC_OneRotate(&gBldc);
            else
                BLDC_NOneRotate(&gBldc);            
        }
        xtos_schedule();
    }
}



int main(void) {
    BLDC_Init(&gBldc);
    
    USART1_Init(115200);
    Hall_Init(&gBldc);
    PWM_Init();
    ADC1_Init((uint16*)&(gBldc.vi));
    IO_Init();
    SPI1_Init();

    NVIC_Configuration();
    
    for (int i = 0; i < 1000; i++)
        _delay(5000);
    
    DRV_Init(&gDrv8305);
    EN_GATE = 0;
    printf("wuhahaha\r\n");

    sys_init_tick(CFG_SYSTICK_PMS);
    xtos_init();
    xtos_init_task_descriptor(&taskA, Cmd_Task, &taskA_Stk[TASKA_STK_SIZE - 1], 0);
    xtos_init_task_descriptor(&taskB, taskb, &taskB_Stk[TASKB_STK_SIZE - 1], 1);
    xtos_start();
}
