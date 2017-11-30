#include <drv8305.h>

/*
 * DRV_ReadRegs - 从DRV8305中获取寄存器信息
 */
static void DRV_ReadRegs(struct Drv8305 *drv) {
    drv->WarningWatchdogReset = SPI1_Communicate(1, 1, 0);      // 0x01 Warning and Watchdog
    drv->OV_VDSFaults = SPI1_Communicate(1, 2, 0);              // 0x02 OV/VDS Faults
    drv->IC_Faults = SPI1_Communicate(1, 3, 0);                 // 0x03 IC Faults
    drv->GateDriverVGSFaults = SPI1_Communicate(1, 4, 0);       // 0x04 Gate Driver VGS Faults
    drv->HSGateDriverControl = SPI1_Communicate(1, 5, 0);       // 0x05 HS Gate Driver
    drv->LSGateDriverControl = SPI1_Communicate(1, 6, 0);       // 0x06 LS Gate Driver
    drv->GateDriveControl = SPI1_Communicate(1, 7, 0);          // 0x07 Gate Driver Control
    drv->ICOperation = SPI1_Communicate(1, 9, 0);               // 0x09 IC Operation
    drv->ShuntAmplifierControl = SPI1_Communicate(1, 0x0A, 0);  // 0x0A Shunt Amplifier Control
    drv->VoltageRegulatorControl = SPI1_Communicate(1, 0x0B, 0);// 0x0B Voltage Regulator Control
    drv->VDSSenseControl = SPI1_Communicate(1, 0x0C, 0);        // 0x0C VDS Sense Control
}
/*
 * DRV_ReadRegs - 向DRV8305的寄存器中写入信息
 */
static void DRV_WriteRegs(struct Drv8305 *drv) {
    SPI1_Communicate(0, 1, drv->WarningWatchdogReset);      // 0x01 Warning and Watchdog
    SPI1_Communicate(0, 2, drv->OV_VDSFaults);              // 0x02 OV/VDS Faults
    SPI1_Communicate(0, 3, drv->IC_Faults);                 // 0x03 IC Faults
    SPI1_Communicate(0, 4, drv->GateDriverVGSFaults);       // 0x04 Gate Driver VGS Faults
    SPI1_Communicate(0, 5, drv->HSGateDriverControl);       // 0x05 HS Gate Driver
    SPI1_Communicate(0, 6, drv->LSGateDriverControl);       // 0x06 LS Gate Driver
    SPI1_Communicate(0, 7, drv->GateDriveControl);          // 0x07 Gate Driver Control
    SPI1_Communicate(0, 9, drv->ICOperation);               // 0x09 IC Operation
    SPI1_Communicate(0, 0x0A, drv->ShuntAmplifierControl);  // 0x0A Shunt Amplifier Control
    SPI1_Communicate(0, 0x0B, drv->VoltageRegulatorControl);// 0x0B Voltage Regulator Control
    SPI1_Communicate(0, 0x0C, drv->VDSSenseControl);        // 0x0C VDS Sense Control
}
/*
 * DRV_Init - 初始化DRV8305芯片
 */
void DRV_Init(struct Drv8305 *drv) {
    SPI1_Communicate(0, 5, 0x3BB);
    SPI1_Communicate(0, 6, 0x3BB);
    SPI1_Communicate(0, 9, 0x322);
    
    DRV_ReadRegs(drv);
    drv->cmd = 0;
}
/*
 * DRV_Exec - 执行针对DRV8305的控制指令
 */
void DRV_Exec(struct Drv8305 *drv) {
    switch (drv->cmd) {
    case DRV_READREGS:
        DRV_ReadRegs(drv);
        break;
    case DRV_WRITEREGS:
        DRV_WriteRegs(drv);
        break;
    }
    drv->cmd = 0;
}
