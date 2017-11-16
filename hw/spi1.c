#include <board.h>

void SPI1_Init(void) {
    // ø™∆Ù ±÷” ∂Àø⁄÷ÿ”≥…‰
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SPI1,ENABLE);
    // GPIO∂Àø⁄≈‰÷√
    GPIO_InitTypeDef GPIO_InitStructure;
    // PB3->SCK, PB4->MISO, PB5->MOSI
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    // PA15->NSS
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // SPI≈‰÷√
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    SPI_InitTypeDef  SPI_InitStructure;  
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;  
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;  
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;  
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;  
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;  
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;  
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; 
    SPI_Init(SPI1, &SPI_InitStructure);  

    SPI_Cmd(SPI1, ENABLE);
    DRV8305_nSELECT;
}

uint16 SPI1_Communicate(uint8 rw, uint8 addr, uint16 data) {
    uint16 cmd = ((rw & 0x01) << 16) + ((addr & 0x0F) << 11) + (data & 0x07FF);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI1->DR = cmd;
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    return SPI1->DR;
}



