#ifndef SYSTEM_H
#define SYSTEM_H

#include <types.h>
#include <config.h>
#include <stm32f10x.h>

#if (1 == CFG_XTOS_USING)
#include <xtos.h>
#endif

struct systicks {
    uint32 ms;      // ms计数
    uint32 ticks;   // systick计数
};

void sys_init_tick(uint32 ticks);
void sys_get_ticks(struct systicks *ticks);
int sys_diff_ticks(const struct systicks *src, const struct systicks *dst);

#endif
