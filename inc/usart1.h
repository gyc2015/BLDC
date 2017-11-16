#ifndef USART1_H
#define USART1_H

#include <types.h>

void USART1_Init(unsigned int bound);
uint8 USART1_SendByte(uint8 value);
uint8 USART1_SendBytes(const uint8 *buf, unsigned int len);

#endif
