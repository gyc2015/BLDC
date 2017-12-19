#ifndef XTOS_TIME_H
#define XTOS_TIME_H

#include <types.h>

void xtos_tick(void);

uint32 xtos_get_ticks(void);

void xtos_set_ticks(uint32 ticks);

void xtos_delay_ticks(uint32 ticks);

#endif
