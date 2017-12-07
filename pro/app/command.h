#ifndef COMMAND_H
#define COMMAND_H

#include <board.h>
#include <queue.h>

struct CmdAddrBits {
    uint16 addr : 15;
    uint16 rw : 1;  // 1:w, 0:r
};
union CmdAddr {
    struct CmdAddrBits bits;
    uint16 all;
};

/*
 * Cmd_ParseStream - �����ַ��������ƥ��@pattern�򷵻�TRUE������FALSE
 *
 * @q: Ŀ�����
 * @pattern: ֡ͷ���ݻ���
 * @len: ֡ͷ����
 */
BOOL Cmd_ParseStream(Queue_T *q, const uint8 *pattern, uint8 len);
/*
 * Cmd_ParseHalfWord - �Ӷ����н�����һ��16λ��
 *
 * @q: Ŀ�����
 */
uint16 Cmd_ParseHalfWord(Queue_T *q);
/*
 * Cmd_ParseBytes - �Ӷ����н�����ָ�����ȵ����ݷŵ�������
 *
 * @q: Ŀ�����
 * @buf: ����
 * @len: ���г���
 */
void Cmd_ParseBytes(Queue_T *q, uint8 *buf, uint16 len);

#endif
