#include <board.h>
#include <utils.h>

void _delay(int n) {
    if (n < 0) n = 1;
    while (n--);
}


/*
* ΢��ͺ��뼶��ʱ����
*/
uint32 fac_us, fac_ms;

/*
* Init_Delay - ��ʱ���ӳ�ʼ������
*
* @sysclk: ϵͳʱ��Ƶ��(MHz)
*/
void Init_Delay(uint8 sysclk) {
    SysTick->CTRL &= 0xfffffff8;
    fac_us = sysclk / 8;
    fac_ms = (u16)fac_us * 1000;
}

/*
* Delay_us - ΢�뼶��ʱ����
*
* @nus: ��ʱ΢����
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
* Delay_ms - ���뼶��ʱ����
*
* @nms: ���뼶��ʱ����, ���벻�ܴ���1000ms
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
* Delay_fms - ���뼶��ʱ����,֧�ָ���������
*
* @fms: ���뼶��ʱ����, ���벻�ܴ���1000ms
*/
void Delay_fms(double fms) {
    uint32 nms = (uint32)fms;
    uint32 nus = (uint32)((fms - nms) * 1000);
    Delay_ms(nms);
    Delay_us(nus);
}

/*
 * Delay_fs - �뼶��ʱ����,֧�ָ���������
 *
 * @fs: ��ʱ����
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


