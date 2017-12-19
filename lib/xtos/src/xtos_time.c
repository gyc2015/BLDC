#include <xtos.h>
#include <xtos_time.h>

uint32 gXtosTicks = 0;

void xtos_tick(void) {
    gXtosTicks++;
    if (XTOS_STATED == xtos_state)
        xtos_schedule();
}

uint32 xtos_get_ticks(void) {
    return gXtosTicks;
}

void xtos_set_ticks(uint32 ticks) {
    gXtosTicks = ticks;
}

void xtos_delay_ticks(uint32 ticks) {
    uint32 origin = gXtosTicks;
    uint32 delta = gXtosTicks - origin;

    while (delta < ticks) {
        xtos_schedule();
        delta = gXtosTicks - origin;
    }
}
