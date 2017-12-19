#include <board.h>
#include <command.h>
#include <xtos.h>

/*
 * Cmd_Expect - ��������@q�е�������@b
 */
static BOOL Cmd_Expect(Queue_T *q, uint8 b) {
    uint8 tmp;
    while (!peek_queue(q, &tmp))
        xtos_schedule();
    return (tmp == b) ? TRUE : FALSE;
}
/*
 * Cmd_ParseStream - �����ַ��������ƥ��@pattern�򷵻�TRUE������FALSE
 *
 * @q: Ŀ�����
 * @pattern: ֡ͷ���ݻ���
 * @len: ֡ͷ����
 */
BOOL Cmd_ParseStream(Queue_T *q, const uint8 *pattern, uint8 len) {
    uint8 tmp;
    
    for (uint8 i = 0; i < len; i++) {
        if (!Cmd_Expect(q, pattern[i])) {
            dequeue(q, &tmp);
            return FALSE;
        }
        dequeue(q, &tmp);
    }
    
    return TRUE;
}
/*
 * Cmd_ParseHalfWord - �Ӷ����н�����һ��16λ��
 *
 * @q: Ŀ�����
 */
uint16 Cmd_ParseHalfWord(Queue_T *q) {
    union Data16 addr;
    
    while (!dequeue(q, &(addr.byte[0])))
        xtos_schedule();
    while (!dequeue(q, &(addr.byte[1])))
        xtos_schedule();
    
    return addr.half_word;
}
/*
 * Cmd_ParseBytes - �Ӷ����н�����ָ�����ȵ����ݷŵ�������
 *
 * @q: Ŀ�����
 * @buf: ����
 * @len: ���г���
 */
void Cmd_ParseBytes(Queue_T *q, uint8 *buf, uint16 len) {
    for (uint16 i = 0; i < len; i++) {
        while (!dequeue(q, &(buf[i])))
            xtos_schedule();
    }
}






