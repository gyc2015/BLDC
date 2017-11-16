#include <board.h>
#include <utils.h>

void _delay(int n) {
    if (n < 0) n = 1;
    while (n--);
}


/*
* 微妙和毫秒级延时因子
*/
uint32 fac_us, fac_ms;

/*
* Init_Delay - 延时因子初始化函数
*
* @sysclk: 系统时钟频率(MHz)
*/
void Init_Delay(uint8 sysclk) {
    SysTick->CTRL &= 0xfffffff8;
    fac_us = sysclk / 8;
    fac_ms = (u16)fac_us * 1000;
}

/*
* Delay_us - 微秒级延时函数
*
* @nus: 延时微秒数
*/
void Delay_us(uint32 nus) {
    uint32 tmp;

    if (0 == nus)
        return;
    SysTick->LOAD = (uint32)nus * fac_us;
    SysTick->VAL = 0x00;
    SysTick->CTRL |= 0x01;
    do {
        tmp = SysTick->CTRL;
    } while (tmp & 0x01 && !(tmp & (1 << 16)));
    SysTick->CTRL &= 0xfffffff8;
    SysTick->VAL = 0x00;
}

/*
* Delay_ms - 毫秒级延时函数
*
* @nms: 毫秒级延时函数, 输入不能大于1000ms
*/
void Delay_ms(uint32 nms) {
    uint32 tmp;

    if (0 == nms)
        return;
    SysTick->LOAD = (uint32)nms * fac_ms;
    SysTick->VAL = 0x00;
    SysTick->CTRL |= 0x01;
    do {
        tmp = SysTick->CTRL;
    } while (tmp & 0x01 && !(tmp & (1 << 16)));
    SysTick->CTRL &= 0xfffffff8;
    SysTick->VAL = 0x00;
}

/*
* Delay_fms - 毫秒级延时函数,支持浮点数输入
*
* @fms: 毫秒级延时函数, 输入不能大于1000ms
*/
void Delay_fms(double fms) {
    uint32 nms = (uint32)fms;
    uint32 nus = (uint32)((fms - nms) * 1000);
    Delay_ms(nms);
    Delay_us(nus);
}

/*
 * Delay_fs - 秒级延时函数,支持浮点数输入
 *
 * @fs: 延时秒数
 */
void Delay_fs(double fs) {
    uint32 ns = (uint32)fs;
    double fms = (fs - ns) * 1000.0;
    int i;
    for (i = 0; i < ns; i++) {
        Delay_ms(1000);
    }
    Delay_fms(fms);
}


