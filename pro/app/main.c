#include <board.h>
#include <command.h>
#include <drv8305.h>
#include <bldc.h>
#include <xtos.h>

int gSCount = 0;

struct Drv8305 gDrv8305;
struct BLDC gBldc;


/*
 * NVIC_Configuration - �ж����ȼ�����
 */
void NVIC_Configuration(void){	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	// USART1 �����ж�
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    // EXTI �ⲿ�ж�
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
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
        DRV_ReadRegs(&gDrv8305);
        if (reg < sizeof(gDrv8305)) {
            ptr = (uint8*)&gDrv8305;
            return ptr[reg];
        }
    }
    return 0;
}


#define TASKA_STK_SIZE 512
#define TASKB_STK_SIZE 512

static uint32 taskA_Stk[TASKA_STK_SIZE];
static uint32 taskB_Stk[TASKB_STK_SIZE];

static struct xtos_task_descriptor taskA;
static struct xtos_task_descriptor taskB;

void taska() {
    uint16 head = 0x55AA;
    union CmdAddr cmd;
    uint16 len = 0;
    uint8 tmptype = 0;
    cmd.all = 0;
    while (1) {
        if (!Cmd_ParseStream(&gU1RxQ, (uint8*)&head, 2))
            continue;
        
        cmd.all = Cmd_ParseHalfWord(&gU1RxQ);
        len = Cmd_ParseHalfWord(&gU1RxQ);
        
        while (len > 0) {
            if (TRUE == CheckAddress(cmd.bits.addr)) {
                if (1 == cmd.bits.rw) {
                    // ������λ����дָ��
                    Cmd_ParseBytes(&gU1RxQ, &tmptype, 1);
                    SetRegister(cmd.bits.addr, tmptype);
                } else {
                    // ������λ���Ķ�ָ��
                    USART1_SendByte(GetRegister(cmd.bits.addr));
                }
            }
            cmd.all++;
            len--;
        }
    }
}

void taskb() {
    while (1) {
        EN_GATE = gBldc.cmd.bits.en;
        if (gBldc.cmd.bits.en) {
            if (gBldc.cmd.bits.dir)
                BLDC_ThreeRotate(&gBldc);
            else
                BLDC_NThreeRotate(&gBldc);            
        }
        xtos_schedule();
    }
}


#define SysTicks_Irq_n 15
/*
 * systick_init - ϵͳʱ�ӳ�ʼ��
 *
 * @ticks: ϵͳʱ��Ƶ��(MHz)
 */
void systick_init(uint32 ticks) {
    SysTick->LOAD = (0x00FFFFFF & ticks);
    SysTick->VAL = 0;

    SCB->SHP[SysTicks_Irq_n - 4] = 0x00;
    
    
    SysTick->CTRL = 0x07;
}


void SysTick_Handler(void) {
    xtos_tick();
}

int main(void) {
    USART1_Init(115200);
    Hall_Init();
    PWM_Init();
    ADC1_Init((uint16*)&(gBldc.vi));
    IO_Init();
    SPI1_Init();

    NVIC_Configuration();

    for (int i = 0; i < 1000; i++)
        _delay(50000);
    
    BLDC_Init(&gBldc);
    DRV_Init(&gDrv8305);
    EN_GATE = 0;
    printf("wuhahaha\r\n");
    
    systick_init(72000);
        
    xtos_init();
    xtos_init_task_descriptor(&taskA, taska, &taskA_Stk[TASKA_STK_SIZE - 1], 0);
    xtos_init_task_descriptor(&taskB, taskb, &taskB_Stk[TASKB_STK_SIZE - 1], 1);
    xtos_start();
}
