#ifndef COMMAND_H
#define COMMAND_H

#include <board.h>
#include <queue.h>

#define CMD_ERROR_NOERROR   0   // û��
#define CMD_ERROR_TIMEOUT   1   // ��ʱ
#define CMD_ERROR_INVALID   2   // �ַ���ƥ��
extern uint8 gCmdError;

BOOL Cmd_CatchByte(Queue_T *q, uint8 *buf);
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
/*
 * Cmd_WaitBytes - �ȴ������д���@len���ֽ�
 *
 * @q: Ŀ�����
 * @len: ���г���
 */
uint16 Cmd_WaitBytes(Queue_T *q, uint16 len);
/****************************************************************************/




#endif
