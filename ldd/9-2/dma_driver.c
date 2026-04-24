#include "LPC17xx.h"

// DMA Channels
#define UART_TX_DMA_CH   0
#define UART_RX_DMA_CH   1

// Buffers (must remain allocated throughout transfer)
uint8_t txBuf[] = "Hello UART0 via DMA!\r\n";
uint8_t rxBuf[64];

// UART0 Init
void UART0_Init(uint32_t baudrate)
{
    LPC_SC->PCONP |= (1 << 3);      // Power on UART0
    LPC_PINCON->PINSEL0 |= (1 << 4) | (1 << 6); // P0.2 TXD0, P0.3 RXD0

    LPC_UART0->LCR  = 0x83;         // 8 bits, no parity, 1 stop, DLAB=1

    uint32_t pclk = SystemCoreClock/4;
    uint16_t dl = (pclk/(16UL*baudrate));
    LPC_UART0->DLL = dl & 0xFF;
    LPC_UART0->DLM = (dl>>8)&0xFF;

    LPC_UART0->LCR  = 0x03;         // DLAB=0
    LPC_UART0->FCR  = 0x07;         // Enable & reset FIFO
}

// GPDMA Init
void GPDMA_Init(void)
{
    LPC_SC->PCONP |= (1 << 29);     // Power on DMA
    LPC_GPDMA->DMACConfig = 1;      // Enable DMA controller
    while ((LPC_GPDMA->DMACConfig & 1) == 0);
    LPC_GPDMA->DMACIntTCClear = 0xFF;
    LPC_GPDMA->DMACIntErrClr  = 0xFF;
}

// UART0 TX via DMA
void UART0_DMA_Tx(uint8_t *buf, uint32_t len)
{
    LPC_GPDMA->DMACIntTCClear = (1 << UART_TX_DMA_CH);
    LPC_GPDMA->DMACIntErrClr  = (1 << UART_TX_DMA_CH);

    LPC_GPDMACH0->DMACCSrcAddr  = (uint32_t)buf;
    LPC_GPDMACH0->DMACCDestAddr = (uint32_t)&LPC_UART0->THR;
    LPC_GPDMACH0->DMACCLLI      = 0;

    LPC_GPDMACH0->DMACCControl =
        (len)        |
        (2 << 18)    | // Src Width = 8-bit
        (2 << 21)    | // Dest Width = 8-bit
        (1 << 26)    | // Src Inc
        (0 << 27);     // Dest No Inc

    LPC_GPDMACH0->DMACCConfig =
        (1 << 0)     | // Enable channel
        (0 << 1)     | // Mem->Periph
        (GPDMA_CONN_UART0_Tx << 6);
}

// UART0 RX via DMA
void UART0_DMA_Rx(uint8_t *buf, uint32_t len)
{
    LPC_GPDMA->DMACIntTCClear = (1 << UART_RX_DMA_CH);
    LPC_GPDMA->DMACIntErrClr  = (1 << UART_RX_DMA_CH);

    LPC_GPDMACH1->DMACCSrcAddr  = (uint32_t)&LPC_UART0->RBR;
    LPC_GPDMACH1->DMACCDestAddr = (uint32_t)buf;
    LPC_GPDMACH1->DMACCLLI      = 0;

    LPC_GPDMACH1->DMACCControl =
        (len)        |
        (2 << 18)    | // Src Width = 8-bit
        (2 << 21)    | // Dest Width = 8-bit
        (0 << 26)    | // Src No Inc
        (1 << 27);     // Dest Inc

    LPC_GPDMACH1->DMACCConfig =
        (1 << 0)     | // Enable channel
        (1 << 1)     | // Periph->Mem
        (GPDMA_CONN_UART0_Rx << 6);
}

int main(void)
{
    SystemInit();
    UART0_Init(115200);
    GPDMA_Init();

    // DMA Transfers
    UART0_DMA_Tx(txBuf, sizeof(txBuf)-1);
    UART0_DMA_Rx(rxBuf, sizeof(rxBuf));

    while (1)
    {
        // CPU can do other work; DMA handles UART
        if (LPC_GPDMA->DMACIntTCStat & (1<<UART_RX_DMA_CH)) {
            LPC_GPDMA->DMACIntTCClear = (1<<UART_RX_DMA_CH);
            // rxBuf now has received data
        }
    }
}
