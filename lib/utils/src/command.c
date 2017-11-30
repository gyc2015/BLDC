#include <board.h>
#include <command.h>

#define HEADINDEX 0
#define ADDINDEX 2
#define LENINDEX 4
#define DATAINDEX 6
#define ENDINDEX 10

Queue_T *g_cmdQueue = 0;
uint8 g_cmdIndex = 0;
uint8 g_cmds[12];

CheckAddrFuncPtr _CheckAddr;
SetRegFuncPtr _SetRegister;
GetRegFuncPtr _GetRegister;
/*******************************************************/

/*
 * Cmd_Init - 初始化CMD指令系统
 *
 * @q: 数据缓存队列
 * @checkFunc: 检查地址合法的函数
 * @setFunc: 写寄存器的函数
 * @getFunc: 读寄存器的函数
 */
void Cmd_Init(Queue_T *q, CheckAddrFuncPtr checkFunc, SetRegFuncPtr setFunc, GetRegFuncPtr getFunc) {
    g_cmdQueue = q;
    _CheckAddr = checkFunc;
    _SetRegister = setFunc;
    _GetRegister = getFunc;
}
/*
 * Cmd_ParseHead - 解析Head
 */
static void Cmd_ParseHead(void) {
    if (is_queue_empty(g_cmdQueue))
        return;
    dequeue(g_cmdQueue, &g_cmds[g_cmdIndex]);
    if (0 == g_cmdIndex && 0xAA != g_cmds[g_cmdIndex])
        g_cmdIndex = HEADINDEX;
    else if (1 == g_cmdIndex && 0x55 != g_cmds[g_cmdIndex])
        g_cmdIndex = HEADINDEX;
    else
        g_cmdIndex++;
}
/*
 * Cmd_ParseAddrLen - 解析地址和数据长度
 */
static void Cmd_ParseAddrLen(void) {
    if (is_queue_empty(g_cmdQueue))
        return;
    dequeue(g_cmdQueue, &g_cmds[g_cmdIndex]);
    g_cmdIndex++;
}
/*
 * Cmd_CatchByte - 从数据缓存中获取一个字节
 */
static uint8 Cmd_CatchByte(void) {
    uint8 tmp = 0;
    dequeue(g_cmdQueue, &tmp);
    return tmp;
}

struct CmdAddrBits {
    uint16 addr : 15;
    uint16 rw : 1;  // 1:w, 0:r
};
union CmdAddr {
    struct CmdAddrBits bits;
    uint16 all;
};
/*
 * Cmd_Parse - 解析指令
 */
void Cmd_Parse(void) {
    // 解析帧头
    if (g_cmdIndex < ADDINDEX)
        Cmd_ParseHead();
    // 解析起始地址和数据长度
    else if (g_cmdIndex < DATAINDEX)
        Cmd_ParseAddrLen();
    else {
        union CmdAddr *paddr = (union CmdAddr *)&g_cmds[ADDINDEX];
        uint16 *pleng = (uint16 *)&g_cmds[LENINDEX];
        if (NULL == _CheckAddr || FALSE == _CheckAddr(paddr->bits.addr) || 0 == (*pleng)) {
            g_cmdIndex = 0;
            return;
        }
        if (1 == paddr->bits.rw) {
            if (is_queue_empty(g_cmdQueue))
                return;
            if (NULL != _SetRegister)
                _SetRegister(paddr->bits.addr, Cmd_CatchByte());
        } else {
            if (NULL != _GetRegister)
                USART1_SendByte(_GetRegister(paddr->bits.addr));
        }
        paddr->all++;
        (*pleng)--;
        g_cmds[0] = 0;
        g_cmds[1] = 0;
    }
}

