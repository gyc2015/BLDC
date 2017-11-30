#ifndef DRV8305_H
#define DRV8305_H

#include <board.h>

#define DRV_READREGS  0x0001
#define DRV_WRITEREGS 0x0002

#pragma pack (1)
struct Drv8305 {
    uint16 cmd;                     // 0
    uint16 WarningWatchdogReset;    // 2
    uint16 OV_VDSFaults;            // 4
    uint16 IC_Faults;               // 6
    uint16 GateDriverVGSFaults;     // 8
    uint16 HSGateDriverControl;     // 10
    uint16 LSGateDriverControl;     // 12
    uint16 GateDriveControl;        // 14
    uint16 ICOperation;             // 16
    uint16 ShuntAmplifierControl;   // 18
    uint16 VoltageRegulatorControl; // 20
    uint16 VDSSenseControl;         // 22
};
#pragma pack ()

void DRV_Init(struct Drv8305 *drv);
void DRV_Exec(struct Drv8305 *drv);

#endif
