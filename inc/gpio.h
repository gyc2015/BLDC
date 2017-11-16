#ifndef GPIO_H
#define GPIO_H

#define EN_GATE     PBout(15)
#define WAKE        PBout(12)
#define PWRGD       PBin(14)
#define nFAULT      PBin(13)


void IO_Init(void);

#endif
