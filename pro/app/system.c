#include <system.h>

#define SysTicks_Irq_n 15

uint32 gSysMs = 0;

/*
 * sys_init_tick - 系统时钟初始化
 *
 * @ticks: 系统时钟频率(MHz)
 */
void sys_init_tick(uint32 ticks) {
    SysTick->LOAD = (0x00FFFFFF & ticks);
    SysTick->VAL = 0;

    SCB->SHP[SysTicks_Irq_n - 4] = 0x00;
    
    SysTick->CTRL = 0x07;
}

void sys_get_ticks(struct systicks *ticks) {
    ticks->ms = gSysMs;
    ticks->ticks = SysTick->VAL;
}

int sys_diff_ticks(const struct systicks *src, const struct systicks *dst) {
    int ticks = dst->ticks - src->ticks;
    int ms = dst->ms - src->ms;
    
    return (CFG_SYSTICK_PMS * ms + ticks);
}


void SysTick_Handler(void) {
    gSysMs++;
#if (1 == CFG_XTOS_USING)
    xtos_tick();
#endif
}




