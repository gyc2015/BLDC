#ifndef BLDC_H
#define BLDC_H

#include <types.h>

/*
 * BldcCmdBits - ��ˢ�������λ����
 */
struct BldcCmdBits {
    uint16 en : 1;      // bit0:�������, 0:�ر�,1:����
    uint16 dir : 1;     // bit1:���ת������, 0:��ת,1:��ת
    uint16 res : 14;    // bit2~15����
};
/*
 * BldcCmd - ��ˢ�������
 */
union BldcCmd {
    struct BldcCmdBits bits;
    uint16 all;
};
/*
 * BldcVISense - Bldc��ѹ��������
 */
struct BldcVISense {
    uint16 pvdd;    // �߼���ѹ
    uint16 cv;      // C�˵�ѹ
    uint16 bv;      // B�˵�ѹ
    uint16 av;      // A�˵�ѹ
    uint16 ai;      // A�����
    uint16 bi;      // B�����
    uint16 ci;      // C�����
};

#define BLDC_DIR_POS     ((int8)(1))
#define BLDC_DIR_NEG     ((int8)(-1))
#define BLDC_DIR_UNKNOWN ((int8)(0))

#pragma pack (1)
struct BLDC {
    union BldcCmd cmd;      // 0-1:���������
    struct BldcVISense vi;  // 2-15:��ѹ����������ֵ
    float duty;             // 16-19: �������ռ�ձ�
    int8 dir;               // 20: ���ת������
    uint8 hall;             // 21: ��ǰhall״̬
    int pulse_count;        // 22-25: hall��������
    float pulse_rate;       // 26-29: hall����Ƶ��(������/ms)
};
#pragma pack ()

void BLDC_Init(struct BLDC *bldc);
void BLDC_NRotate(struct BLDC *bldc);
void BLDC_Rotate(struct BLDC *bldc);

/*
 * BLDC_NThreeRotate - ������ͨ��תBLDC
 */
void BLDC_NOneRotate(struct BLDC *bldc);
void BLDC_OneRotate(struct BLDC *bldc);

#endif
