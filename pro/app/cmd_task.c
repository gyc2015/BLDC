#include <cmd_task.h>
#include <board.h>
#include <command.h>
#include <drv8305.h>
#include <bldc.h>
#include <xtos.h>

extern struct Drv8305 gDrv8305;
extern struct BLDC gBldc;

BOOL CheckAddress(uint16 add, BOOL isWrite) {
    uint8 page = (uint8)((add & 0xFF00) >> 8);    

    switch (page) {
    case CFG_REGPAGE_COMMAND: return TRUE;
    case CFG_REGPAGE_DRV8305: return TRUE;
    }
    
    return FALSE;
}

void SetRegister(uint16 addr, uint8 data) {
    uint8 page = (uint8)((addr & 0xFF00) >> 8);
    uint8 reg = (uint8)(addr & 0xFF);
    uint8 *ptr = NULL;
    
    if (CFG_REGPAGE_COMMAND == page) {
        if (reg < sizeof(gBldc)) {
            ptr = (uint8*)&gBldc;
            ptr[reg] = data;
        }
    } else if (CFG_REGPAGE_DRV8305 == page) {
        if (reg < sizeof(gDrv8305)) {
            ptr = (uint8*)&gDrv8305;
            ptr[reg] = data;
        }
        
        if (0 == reg || 1 == reg)
            DRV_Exec(&gDrv8305);
    }
}

uint8 GetRegister(uint16 addr) {
    uint8 page = (uint8)((addr & 0xFF00) >> 8);
    uint8 reg = (uint8)(addr & 0xFF);
    uint8 *ptr = NULL;
    
    if (CFG_REGPAGE_COMMAND == page) {
        if (reg < sizeof(gBldc)) {
            ptr = (uint8*)&gBldc;
            return ptr[reg];
        }
    } else if (CFG_REGPAGE_DRV8305 == page) {
        if (reg < sizeof(gDrv8305)) {
            ptr = (uint8*)&gDrv8305;
            return ptr[reg];
        }
    }
    return 0;
}



uint16 _cmdHead = 0x55AA;
uint16 _cmdEnd = 0xAA55;

union CmdAddr _cmd;
uint16 _len = 0;
uint8 _cmdbuf[CFG_CMD_BUF_SIZE];
/*
 * Cmd_CheckStates - 检查Cmd指令状态
 *
 * pfunc: 指令状态函数
 */
static BOOL Cmd_CheckStates(BOOL (*pfunc)(void)) {
    uint16 times = 0;
    
    while (!pfunc()) {
        times++;
        if (times > CFG_CMD_TIMEOUT)
            return FALSE;
        xtos_schedule();
    }
    return TRUE;
}

/*
 * Cmd_InitState - 电机处于Init状态,等待来自上位机的指令握手信号
 */
static BOOL Cmd_InitState(void) {
    while (!Cmd_ParseStream(&gU1RxQ, (uint8*)&_cmdHead, 2));
    
    _cmd.all = Cmd_ParseHalfWord(&gU1RxQ);
    if (CMD_ERROR_NOERROR != gCmdError)
        return FALSE;
    
    _len = Cmd_ParseHalfWord(&gU1RxQ);
    if (CMD_ERROR_NOERROR != gCmdError)
        return FALSE;
    
    if (!Cmd_ParseStream(&gU1RxQ, (uint8*)&_cmdEnd, 2))
        return FALSE;
    
    return TRUE;
}
/*
 * Cmd_HandShakeSend - 电机处于HandShake状态,向上位机发送指令响应
 */
static BOOL Cmd_HandShakeSend(void) {
    USART1_SendBytes((uint8*)&_cmdHead, 2);
    USART1_SendBytes((uint8*)&_cmd, 2);
    USART1_SendBytes((uint8*)&_len, 2);
    USART1_SendBytes((uint8*)&_cmdEnd, 2);
    return TRUE;
}
/*
 * Cmd_HandShakeState - 电机处于HandShake状态,等待上位机发送握手校验信号
 *
 * 接收到0x00A0才标识握手成功，可以开始通信，否则返回初始状态
 */
static BOOL Cmd_HandShakeState(void) {
    if (!Cmd_ParseStream(&gU1RxQ, (uint8*)&_cmdHead, 2))
        return FALSE;
    
    uint16 data = Cmd_ParseHalfWord(&gU1RxQ);
    if (CMD_ERROR_NOERROR != gCmdError)
        return FALSE;
    
    if (0x00A0 != data)
        return FALSE;
    
    if (!Cmd_ParseStream(&gU1RxQ, (uint8*)&_cmdEnd, 2))
        return FALSE;

    return TRUE;
}
/*
 * Cmd_SendingData - 发送数据
 */
static BOOL Cmd_SendingData(void) {
    uint8 sum = 0;
    uint8 tmp = 0;
    USART1_SendBytes((uint8*)&_cmdHead, 2);
    while (_len > 0) {
        if (TRUE == CheckAddress(_cmd.bits.addr, FALSE)) {
            tmp = GetRegister(_cmd.bits.addr);
            sum += tmp;
            USART1_SendByte(tmp);
        }
        _cmd.all++;
        _len--;
    }
    USART1_SendByte(sum);
    USART1_SendBytes((uint8*)&_cmdEnd, 2);
    return TRUE;
}
/*
 * Cmd_ReceivingData - 接收数据
 */
static BOOL Cmd_ReceivingData(void) {
    if (!Cmd_ParseStream(&gU1RxQ, (uint8*)&_cmdHead, 2))
        return FALSE;
    
    uint8 tmp = 0;
    uint8 sum = 0;
    for (int i = 0; i < _len; i++) {
        if (!Cmd_CatchByte(&gU1RxQ, &tmp))
            return FALSE;
        _cmdbuf[i] = tmp;
        sum += tmp;
    }
    if (!Cmd_CatchByte(&gU1RxQ, &tmp))
        return FALSE;
    if (sum != tmp)
        return FALSE;
    
    if (!Cmd_ParseStream(&gU1RxQ, (uint8*)&_cmdEnd, 2))
        return FALSE;
    
    return TRUE;
}

/*
 * Cmd_HandShakeSend - 电机处于HandShake状态,向上位机发送指令响应
 */
static BOOL Cmd_RecvStateSend(uint16 ack) {
    USART1_SendBytes((uint8*)&_cmdHead, 2);
    USART1_SendBytes((uint8*)&ack, 2);
    USART1_SendBytes((uint8*)&_cmdEnd, 2);
    return TRUE;
}


void Cmd_Task(void) {
    while (1) {
        if (!Cmd_InitState()) {
            xtos_schedule();
            continue;
        }
        
        Cmd_HandShakeSend();

        if (!Cmd_CheckStates(Cmd_HandShakeState))
            continue;
        
        if (0 == _cmd.bits.rw) {
            Cmd_SendingData();
        } else {
            if (!Cmd_ReceivingData()) {
                Cmd_RecvStateSend(0x000A);
            } else {
                Cmd_RecvStateSend(0x00A0);
            }
        }
    }
}
