#include <xtos.h>

struct xtos_task_descriptor *gp_xtos_cur_task;
struct xtos_task_descriptor *gp_xtos_next_task;

struct list_head L0_tasks;

void xtos_first_switch(void);
void xtos_context_switch(void);
void xtos_pendsv_handler(void);
/*
 * xtos_init - ��ʼ������ϵͳ
 *
 * 1. ��ʼ���������
 */
void xtos_init() {
    init_list_head(&L0_tasks);
}
/*
 * xtos_task_finished - ���������Ļص�����
 */
void xtos_distroy_task() {
    // to do...
    while(1){}
}
/*
 * xtos_create_task - ����һ�����񣬳�ʼ������ջ�ռ�
 *
 * @tcb: ����������
 * @task: ������ں���
 * @stk: ����ջ��
 */
static uint32* xtos_create_task(struct xtos_task_descriptor * tcb, xtos_task task, uint32 * stk) {
    uint32  *pstk;
    pstk = stk;
    pstk = (uint32 *)((uint32)(pstk) & 0xFFFFFFF8uL);

    *(--pstk) = (uint32)0x01000000uL; // xPSR
    *(--pstk) = (uint32)task;         // Entry Point
    *(--pstk) = (uint32)xtos_distroy_task; // R14 (LR)
    *(--pstk) = (uint32)0x12121212uL; // R12
    *(--pstk) = (uint32)0x03030303uL; // R3
    *(--pstk) = (uint32)0x02020202uL; // R2
    *(--pstk) = (uint32)0x01010101uL; // R1
    *(--pstk) = (uint32)0x00000000u;  // R0

    *(--pstk) = (uint32)0x11111111uL; // R11
    *(--pstk) = (uint32)0x10101010uL; // R10
    *(--pstk) = (uint32)0x09090909uL; // R9
    *(--pstk) = (uint32)0x08080808uL; // R8
    *(--pstk) = (uint32)0x07070707uL; // R7
    *(--pstk) = (uint32)0x06060606uL; // R6
    *(--pstk) = (uint32)0x05050505uL; // R5
    *(--pstk) = (uint32)0x04040404uL; // R4

    tcb->pTopOfStack = pstk;
    return pstk;
}
/*
 * xtos_init_task_struct - ����һ�����񣬳�ʼ������ջ�ռ�
 *
 * @tcb: ����������
 * @task: ������ں���
 * @stk_bottom: ����ջ��
 * @pid: ����id
 */
void xtos_init_task_descriptor(struct xtos_task_descriptor *tcb, xtos_task task, uint32 *stk_bottom, uint16 pid) {
    tcb->pBottomOfStack = stk_bottom;
    tcb->pTopOfStack = xtos_create_task(tcb, task, stk_bottom);
    tcb->pid = pid;

    init_list_head(&tcb->list);
    list_add_tail(&tcb->list, &L0_tasks);
}

/*
 * xtos_schedule - ϵͳ������
 */
void xtos_schedule(void) {
    __asm("CPSID    I");
    list_add_tail(&gp_xtos_next_task->list, &L0_tasks);

    gp_xtos_next_task = list_first_entry(&L0_tasks, struct xtos_task_descriptor, list);

    list_del(&gp_xtos_next_task->list);

    xtos_context_switch();
    __asm("CPSIE   I");
}
/*
 * xtos_start - ��������ϵͳ
 */
void xtos_start(void) {
    __asm("CPSID    I");
    gp_xtos_next_task = list_first_entry(&L0_tasks, struct xtos_task_descriptor, list);

    list_del(&gp_xtos_next_task->list);

    xtos_first_switch();
    __asm("CPSIE   I");
}
