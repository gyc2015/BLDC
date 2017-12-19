#ifndef BLDC_H
#define BLDC_H

#include <types.h>

/*
 * BldcCmdBits - 无刷电机控制位定义
 */
struct BldcCmdBits {
    uint16 en : 1;      // bit0:开启电机, 0:关闭,1:开启
    uint16 dir : 1;     // bit1:电机转动方向, 0:正转,1:反转
    uint16 res : 14;    // bit2~15保留
};
/*
 * BldcCmd - 无刷电机控制
 */
union BldcCmd {
    struct BldcCmdBits bits;
    uint16 all;
};
/*
 * BldcVISense - Bldc电压电流采样
 */
struct BldcVISense {
    uint16 pvdd;    // 逻辑电压
    uint16 cv;      // C端电压
    uint16 bv;      // B端电压
    uint16 av;      // A端电压
    uint16 ai;      // A相电流
    uint16 bi;      // B相电流
    uint16 ci;      // C相电流
};

#define BLDC_DIR_POS     ((int8)(1))
#define BLDC_DIR_NEG     ((int8)(-1))
#define BLDC_DIR_UNKNOWN ((int8)(0))

#pragma pack (1)
struct BLDC {
    union BldcCmd cmd;      // 0-1:电机控制字
    struct BldcVISense vi;  // 2-15:电压电流采样数值
    float duty;             // 16-19: 电机控制占空比
    int8 dir;               // 20: 电机转动方向
    uint8 hall;             // 21: 当前hall状态
    int pulse_count;        // 22-25: hall脉冲数量
    float pulse_rate;       // 26-29: hall脉冲频率(脉冲数/ms)
};
#pragma pack ()

void BLDC_Init(struct BLDC *bldc);
void BLDC_NRotate(struct BLDC *bldc);
void BLDC_Rotate(struct BLDC *bldc);

/*
 * BLDC_NThreeRotate - 三三导通反转BLDC
 */
void BLDC_NOneRotate(struct BLDC *bldc);
void BLDC_OneRotate(struct BLDC *bldc);

#endif
