#ifndef CONFIG_H
#define CONFIG_H

#define CFG_XTOS_USING      1
/******************************************************************
 * 系统时钟设置
 ******************************************************************/
#define CFG_SYSTICK_PMS     (72000)
#define CFG_SYSTICK_MSP     (1.0 / 72000.0)

/******************************************************************
 * 寄存器的页划分
 ******************************************************************/
#define CFG_REGPAGE_COMMAND 0x00
#define CFG_REGPAGE_DRV8305 0x0F

/******************************************************************
 * 串口配置项
 ******************************************************************/
#define CFG_UART_PORT       USART1
#define CFG_UART_BAUD_RATE  115200
#define CFG_UART_BUF_SIZE   32
#define CFG_UART_SEND_TIMEOUT 1000
/******************************************************************
 * 指令系统配置项
 ******************************************************************/
#define CFG_CMD_TIMEOUT     100
#define CFG_CMD_BUF_SIZE    256
/******************************************************************
 * 电机频率配置项
 ******************************************************************/




#endif
