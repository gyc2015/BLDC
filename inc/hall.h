#ifndef HALL_H
#define HALL_H

#include <types.h>

#define HallA   PBin(14)
#define HallB   PBin(13)
#define HallC   PBin(12)

extern int32 gCount;

void Hall_Init(void);
uint8 Hall_GetStatus(void);

#endif
