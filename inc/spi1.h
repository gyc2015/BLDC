#ifndef SPI1_H
#define SPI1_H

#include <types.h>

#define DRV8305_SELECT (PAout(15) = 0)
#define DRV8305_nSELECT (PAout(15) = 1)

void SPI1_Init(void);
uint16 SPI1_Communicate(uint8 rw, uint8 addr, uint16 data);

#endif
