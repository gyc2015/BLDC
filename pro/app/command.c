#include <board.h>
#include <command.h>
#include <xtos.h>

/*
 * Cmd_Expect - 期望队列@q中的数据是@b
 */
static BOOL Cmd_Expect(Queue_T *q, uint8 b) {
    uint8 tmp;
    while (!peek_queue(q, &tmp))
        xtos_schedule();
    return (tmp == b) ? TRUE : FALSE;
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
            dequeue(q, &tmp);
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
    
    while (!dequeue(q, &(addr.byte[0])))
        xtos_schedule();
    while (!dequeue(q, &(addr.byte[1])))
        xtos_schedule();
    
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






