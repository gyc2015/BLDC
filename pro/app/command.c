#include <board.h>
#include <command.h>
#include <xtos.h>

uint8 gCmdError = CMD_ERROR_NOERROR;
/*
 * Cmd_Expect - ��������@q�е�������@b
 */
static BOOL Cmd_Expect(Queue_T *q, uint8 b) {
    uint8 tmp;
    uint16 times = 0;
    while (!peek_queue(q, &tmp)) {
        times++;
        if (times > CFG_CMD_TIMEOUT) {
            gCmdError = CMD_ERROR_TIMEOUT;
            return FALSE;
        }
        xtos_schedule();
    }
    
    if (tmp != b) {
        gCmdError = CMD_ERROR_INVALID;
        return FALSE;
    } else {
        gCmdError = CMD_ERROR_NOERROR;
        return TRUE;
    }
}
/*
 * Cmd_CatchByte - ����һ���ֽ�
 *
 * @q: Ŀ�����
 * @buf: �������ݻ���
 */
BOOL Cmd_CatchByte(Queue_T *q, uint8 *buf) {
    uint16 times = 0;
    
    while (!dequeue(q, buf)) {
        times++;
        if (times > CFG_CMD_TIMEOUT) {
            gCmdError = CMD_ERROR_TIMEOUT;
            return FALSE;
        }
        xtos_schedule();
    }
    
    gCmdError = CMD_ERROR_NOERROR;
    return TRUE;
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
            clear_queue(q);
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
    
    if (!Cmd_CatchByte(q, &(addr.byte[0])))
        return 0;
    if (!Cmd_CatchByte(q, &(addr.byte[1])))
        return 0;
    
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
/*
 * Cmd_WaitBytes - �ȴ������д���@len���ֽ�
 *
 * @q: Ŀ�����
 * @len: ���г���
 */
uint16 Cmd_WaitBytes(Queue_T *q, uint16 len) {
    uint16 times = 0;
    
    while (len > get_queue_size(q)) {
        times++;
        if (times > CFG_CMD_TIMEOUT) {
            gCmdError = CMD_ERROR_TIMEOUT;
            return FALSE;
        }
        xtos_schedule();
    }
    
    gCmdError = CMD_ERROR_NOERROR;
    return TRUE;
}




