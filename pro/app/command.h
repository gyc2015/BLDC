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

#endif
