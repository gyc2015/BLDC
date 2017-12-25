#include <board.h>
#include <command.h>
#include <xtos.h>

uint8 gCmdError = CMD_ERROR_NOERROR;
/*
 * Cmd_Expect - 期望队列@q中的数据是@b
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
 * Cmd_CatchByte - 捕获一个字节
 *
 * @q: 目标队列
 * @buf: 捕获内容缓存
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
 * Cmd_ParseStream - 解析字符流，如果匹配@pattern则返回TRUE，否则FALSE
 *
 * @q: 目标队列
 * @pattern: 帧头内容缓存
 * @len: 帧头长度
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
 * Cmd_ParseHalfWord - 从队列中解析出一个16位数
 *
 * @q: 目标队列
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
 * Cmd_ParseBytes - 从队列中解析出指定长度的内容放到缓存中
 *
 * @q: 目标队列
 * @buf: 缓存
 * @len: 队列长度
 */
void Cmd_ParseBytes(Queue_T *q, uint8 *buf, uint16 len) {
    for (uint16 i = 0; i < len; i++) {
        while (!dequeue(q, &(buf[i])))
            xtos_schedule();
    }
}
/*
 * Cmd_WaitBytes - 等待队列中存在@len个字节
 *
 * @q: 目标队列
 * @len: 队列长度
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




