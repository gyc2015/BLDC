#ifndef PID_H
#define PID_H

/*
 * pid_params - PID�Ĳ�������(ͨ��)
 */
struct pid_params {
    double Kp;  /* ����ϵ�� */
    double Ti;  /* ����ϵ�� */
    double Td;  /* ΢��ϵ�� */
    double T;   /* �������� */
};

/*
 * pid_params_inc - ����ʽ�㷨���κ��PID����
 */
struct pid_params_inc {
    double A;   /* A = 1 + (T/Ti) + (Td/T) */
    double B;   /* B = 1 + 2Td/T           */
    double C;   /* C = KpTd/T              */
};

/*
 * pid_params_general2inc - ͨ��PID����ת��Ϊ����ʽ����
 *
 * @general: ͨ��PID����
 * @inc: ����ʽPID����
 */
void pid_params_general2inc(const struct pid_params *general, struct pid_params_inc *inc);

/*
 * pid_errors - ����ʽPID�㷨���ƹ�ʽ��ǰ�����ε�ƫ��ֵ
 */
struct pid_errors {
    double ek;      /* e(k)   */
    double ek_1;    /* e(k-1) */
    double ek_2;    /* e(k-2) */
};

/*
 * pid_inc_uk - ����ʽPID�㷨
 *
 * @p: ����ʽPID����
 * @e: ǰ�����ε�ƫ��ֵ�������˳�����½ṹ�� ek_2 = ek_1, ek_1 = ek, ek = 0
 */
double pid_inc_uk(const struct pid_params_inc *p, struct pid_errors *e);

#endif

