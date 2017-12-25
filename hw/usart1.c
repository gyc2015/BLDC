/**********************************************
 * 串口1
 *
 * 接入PB6,7需要引脚重映射
 *********************************************/
#include <board.h>

void USART1_GPIO_Init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    // GPIO端口配置
    GPIO_InitTypeDef GPIO_InitStructure;
    // PB6->Tx 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    // PB7->Rx
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/*
 * USART1_Init - 初始化串口1
 *
 * @bound: 串口1的初始化波特率
 */
void USART1_Init(unsigned int baud) {
    // 开启时钟 端口重映射
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);
    // GPIO端口配置
    GPIO_InitTypeDef GPIO_InitStructure;
    // PB6->Tx 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    // PB7->Rx
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    // USART时钟配置
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    USART_ClockInitTypeDef USART_ClockInitStructure;
    USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
    USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
    USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
    USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
    USART_ClockInit(USART1, &USART_ClockInitStructure);
    // USART初始化配置
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = baud;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);
    
    init_queue(&gU1RxQ, gU1RxQ_Buf, CFG_UART_BUF_SIZE);
}

/*
 * USART1_IRQHandler - 串口1的中断处理函数
 */
void USART1_IRQHandler(void) {
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        enqueue(&gU1RxQ, USART_ReceiveData(USART1));
    }
}

/*
 * USART3_SendByte - 从串口发送一个byte的数据
 *
 * @value: 目标数据
 */
uint8 USART1_SendByte(uint8 value) {
    uint16 times = 0;
    USART1->DR = value;
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) {
        times++;
        if (times > CFG_UART_SEND_TIMEOUT)
            return 1;
    }
    return 0;
}

/*
 * USART3_SendBytes - 从串口发送byte数据
 *
 * @buf: 数据缓存
 * @len: 数据长度
 */
uint8 USART1_SendBytes(const uint8 *buf, unsigned int len) {
    for (int i = 0; i < len; i++) {
        if (0 != USART1_SendByte(buf[i]))
            return 1;
    }
    return 0;
}



