#ifndef COMMAND_H
#define COMMAND_H

#include <board.h>
#include <queue.h>

#define CMD_ERROR_NOERROR   0   // 没错
#define CMD_ERROR_TIMEOUT   1   // 超时
#define CMD_ERROR_INVALID   2   // 字符不匹配
extern uint8 gCmdError;

BOOL Cmd_CatchByte(Queue_T *q, uint8 *buf);
/*
 * Cmd_ParseStream - 解析字符流，如果匹配@pattern则返回TRUE，否则FALSE
 *
 * @q: 目标队列
 * @pattern: 帧头内容缓存
 * @len: 帧头长度
 */
BOOL Cmd_ParseStream(Queue_T *q, const uint8 *pattern, uint8 len);
/*
 * Cmd_ParseHalfWord - 从队列中解析出一个16位数
 *
 * @q: 目标队列
 */
uint16 Cmd_ParseHalfWord(Queue_T *q);
/*
 * Cmd_ParseBytes - 从队列中解析出指定长度的内容放到缓存中
 *
 * @q: 目标队列
 * @buf: 缓存
 * @len: 队列长度
 */
void Cmd_ParseBytes(Queue_T *q, uint8 *buf, uint16 len);
/*
 * Cmd_WaitBytes - 等待队列中存在@len个字节
 *
 * @q: 目标队列
 * @len: 队列长度
 */
uint16 Cmd_WaitBytes(Queue_T *q, uint16 len);
/****************************************************************************/




#endif
