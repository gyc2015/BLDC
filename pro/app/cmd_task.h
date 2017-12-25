#ifndef CMD_TASK_H
#define CMD_TASK_H

#include <types.h>

struct CmdAddrBits {
    uint16 addr : 15;
    uint16 rw : 1;  // 1:w, 0:r
};
union CmdAddr {
    struct CmdAddrBits bits;
    uint16 all;
};


void Cmd_Task(void);

#endif
