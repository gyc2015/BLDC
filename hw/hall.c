#include <board.h>

/*
 * Init_Hall - ��ʼ������������
 */
void Hall_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/*
 * Hall_GetStatus - ��ȡ����״̬
 */
uint8 Hall_GetStatus(void) {
    return ((GPIO_ReadInputData(GPIOB) >> 12) & 0x07);
}


