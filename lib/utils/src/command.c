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
 * Cmd_Init - ��ʼ��CMDָ��ϵͳ
 *
 * @q: ���ݻ������
 * @checkFunc: ����ַ�Ϸ��ĺ���
 * @setFunc: д�Ĵ����ĺ���
 * @getFunc: ���Ĵ����ĺ���
 */
void Cmd_Init(Queue_T *q, CheckAddrFuncPtr checkFunc, SetRegFuncPtr setFunc, GetRegFuncPtr getFunc) {
    g_cmdQueue = q;
    _CheckAddr = checkFunc;
    _SetRegister = setFunc;
    _GetRegister = getFunc;
}
/*
 * Cmd_ParseHead - ����Head
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
 * Cmd_ParseAddrLen - ������ַ�����ݳ���
 */
static void Cmd_ParseAddrLen(void) {
    if (is_queue_empty(g_cmdQueue))
        return;
    dequeue(g_cmdQueue, &g_cmds[g_cmdIndex]);
    g_cmdIndex++;
}
/*
 * Cmd_CatchByte - �����ݻ����л�ȡһ���ֽ�
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
 * Cmd_Parse - ����ָ��
 */
void Cmd_Parse(void) {
    // ����֡ͷ
    if (g_cmdIndex < ADDINDEX)
        Cmd_ParseHead();
    // ������ʼ��ַ�����ݳ���
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

