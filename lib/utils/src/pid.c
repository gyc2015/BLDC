#include <pid.h>
#include <assert.h>

/*
 * pid_params_general2inc - ͨ��PID����ת��Ϊ����ʽ����
 *
 * @general: ͨ��PID����
 * @inc: ����ʽPID����
 */
void pid_params_general2inc(const struct pid_params *general, struct pid_params_inc *inc) {
    if (0 == general->Ti)
        inc->A = 1 + (general->Td / general->T);
    else
        inc->A = 1 + (general->T / general->Ti) + (general->Td / general->T);
    inc->A = general->Kp * inc->A;
    inc->B = -general->Kp * (1 + 2 * general->Td / general->T);
    inc->C = general->Kp * general->Td / general->T;
}

/*
* pid_inc_uk - ����ʽPID�㷨
*
* @p: ����ʽPID����
* @e: ǰ�����ε�ƫ��ֵ�������˳�����½ṹ�� ek_2 = ek_1, ek_1 = ek, ek = 0
*/
double pid_inc_uk(const struct pid_params_inc *p, struct pid_errors *e) {
    double re = (p->A * e->ek) + (p->B * e->ek_1) + (p->C * e->ek_2);
    e->ek_2 = e->ek_1;
    e->ek_1 = e->ek;
    e->ek = 0;

    return re;
}

