#ifndef GPIO_H
#define GPIO_H

#define EN_GATE     PAout(12)
#define WAKE        PBout(2)
#define PWRGD       PBin(0)
#define nFAULT      PBin(1)


void IO_Init(void);

#endif
