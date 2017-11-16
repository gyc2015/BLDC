#include <board.h>
#include <rt_misc.h>

#pragma import(__use_no_semihosting_swi)

int SendChar(int ch) {
    while (!(CFG_UART_PORT->SR & USART_FLAG_TXE));
    CFG_UART_PORT->DR = (ch & 0x1FF);
    return (ch);
}

int GetKey(void) {
    while (!(CFG_UART_PORT->SR & USART_FLAG_RXNE));
    return ((int)(CFG_UART_PORT->DR & 0x1FF));
}


struct __FILE {
    int handle; // Add whatever you need here
};

FILE __stdout;

FILE __stdin;

int fputc(int ch, FILE *f) {
    return SendChar(ch);
}

int fgetc(FILE *f) {
    return GetKey();
}

void _ttywrch(int ch) {
    SendChar(ch);
}

int ferror(FILE *f) { // Your implementation of ferror
    return EOF;
}

void _sys_exit(int return_code) {
label: goto label; // endless loop
}

