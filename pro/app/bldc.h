#ifndef BLDC_H
#define BLDC_H

#include <board.h>

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

#pragma pack (1)
struct BLDC {
    union BldcCmd cmd;      // 0:���������
    struct BldcVISense vi;  // 2-14:��ѹ����������ֵ
    float duty;             // 16: �������ռ�ձ�
    
};
#pragma pack ()

void BLDC_Init(struct BLDC *bldc);
void BLDC_NRotate(struct BLDC *bldc);
void BLDC_Rotate(struct BLDC *bldc);

#endif
