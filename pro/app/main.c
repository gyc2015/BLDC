#include <board.h>

uint8 tmp = 0;

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


int main(void) {
    USART1_Init(115200);
    
    printf("wuhahaha\r\n");

    NVIC_Configuration();
    
    while (1) {
        while (!is_queue_empty(&gU1RxQ)) {
            dequeue(&gU1RxQ, &tmp);
            USART1_SendByte(tmp);
        }
    }
}

