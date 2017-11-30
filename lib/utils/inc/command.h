#ifndef COMMAND_H
#define COMMAND_H

#include <board.h>

typedef BOOL (*CheckAddrFuncPtr)(uint16 addr);
typedef void (*SetRegFuncPtr)(uint16 addr, uint8 data);
typedef uint8 (*GetRegFuncPtr)(uint16 addr);

void Cmd_Init(Queue_T *q, CheckAddrFuncPtr checkFunc, SetRegFuncPtr setFunc, GetRegFuncPtr getFunc);
void Cmd_Parse(void);

#endif
