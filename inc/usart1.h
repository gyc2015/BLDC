#ifndef USART1_H
#define USART1_H

#include <types.h>

#define USART1_KEY_1 PBin(6)
#define USART1_KEY_2 PBin(7)

void USART1_GPIO_Init(void);
void USART1_Init(unsigned int bound);
uint8 USART1_SendByte(uint8 value);
uint8 USART1_SendBytes(const uint8 *buf, unsigned int len);

#endif
