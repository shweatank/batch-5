#include <stdint.h>

/* ================= LPC1768 GPDMA Register Base ================= */
#define GPDMA_BASE        0x50004000UL
#define GPDMA_INTSTAT     (*(volatile uint32_t *)(GPDMA_BASE + 0x000))
#define GPDMA_CONFIG      (*(volatile uint32_t *)(GPDMA_BASE + 0x030))
#define GPDMA_SYNC        (*(volatile uint32_t *)(GPDMA_BASE + 0x034))

/* ================= DMA Channel 0 Registers ================= */
#define GPDMA_CH0_SRC     (*(volatile uint32_t *)(GPDMA_BASE + 0x100))
#define GPDMA_CH0_DEST    (*(volatile uint32_t *)(GPDMA_BASE + 0x104))
#define GPDMA_CH0_LLI     (*(volatile uint32_t *)(GPDMA_BASE + 0x108))
#define GPDMA_CH0_CTRL    (*(volatile uint32_t *)(GPDMA_BASE + 0x10C))
#define GPDMA_CH0_CFG     (*(volatile uint32_t *)(GPDMA_BASE + 0x110))

/* ================= Test Buffers ================= */
#define DMA_TRANSFER_SIZE 16

uint32_t src_buffer[DMA_TRANSFER_SIZE] =
{
    1,2,3,4,5,6,7,8,
    9,10,11,12,13,14,15,16
};

uint32_t dest_buffer[DMA_TRANSFER_SIZE];

/* ================= DMA Init ================= */
void dma_init(void)
{
    /* Enable GPDMA controller */
    GPDMA_CONFIG = 0x01;

    /* Small delay to allow DMA clock to stabilize */
    for (volatile int i = 0; i < 1000; i++);
}

/* ================= DMA Start ================= */
void dma_memcpy(uint32_t *src, uint32_t *dst, uint32_t length)
{
    /* Disable channel before configuration */
    GPDMA_CH0_CFG = 0;

    /* Source and destination addresses */
    GPDMA_CH0_SRC  = (uint32_t)src;
    GPDMA_CH0_DEST = (uint32_t)dst;
    GPDMA_CH0_LLI  = 0;   /* No linked list */

    /* Control Register:
       - Transfer size = length
       - Source width = 32-bit
       - Destination width = 32-bit
       - Source increment = yes
       - Destination increment = yes
    */
    GPDMA_CH0_CTRL =
        (length & 0xFFF) |       /* Transfer size */
        (2 << 18) |              /* Source width = 32-bit */
        (2 << 21) |              /* Destination width = 32-bit */
        (1 << 26) |              /* Source increment */
        (1 << 27);               /* Destination increment */

    /* Configuration Register:
       - Enable channel
       - Memory to Memory transfer
    */
    GPDMA_CH0_CFG =
        (1 << 0) |               /* Enable channel */
        (0 << 1) |               /* Src peripheral = memory */
        (0 << 6) |               /* Dest peripheral = memory */
        (0 << 11);               /* Flow control: DMA */
}

/* ================= DMA Status ================= */
uint8_t dma_is_busy(void)
{
    return (GPDMA_INTSTAT & 0x01);
}



/*
🔹 Register Base
#define GPDMA_BASE 0x50004000UL


➡ Base address of GPDMA controller from LPC1768 datasheet.

🔹 Global DMA Registers
#define GPDMA_INTSTAT (*(volatile uint32_t *)(GPDMA_BASE + 0x000))


➡ Shows which DMA channels are active.

#define GPDMA_CONFIG (*(volatile uint32_t *)(GPDMA_BASE + 0x030))


➡ Master enable register for DMA.

🔹 Channel 0 Registers
#define GPDMA_CH0_SRC (*(volatile uint32_t *)(GPDMA_BASE + 0x100))


➡ Source address for DMA channel 0.

#define GPDMA_CH0_DEST (*(volatile uint32_t *)(GPDMA_BASE + 0x104))


➡ Destination address.

#define GPDMA_CH0_CTRL (*(volatile uint32_t *)(GPDMA_BASE + 0x10C))


➡ Most important register:

Transfer size

Data width

Increment control

🔹 Test Buffers
uint32_t src_buffer[DMA_TRANSFER_SIZE];
uint32_t dest_buffer[DMA_TRANSFER_SIZE];


➡ Source and destination memory areas for DMA copy.

🔹 DMA Initialization
GPDMA_CONFIG = 0x01;


➡ Enables DMA controller globally.

for (volatile int i = 0; i < 1000; i++);


➡ Hardware settling delay (common in low-level drivers).

🔹 DMA Configuration Function
GPDMA_CH0_CFG = 0;


➡ Always disable channel before reconfiguring.

🔹 Address Setup
GPDMA_CH0_SRC  = (uint32_t)src;
GPDMA_CH0_DEST = (uint32_t)dst;


➡ Tells DMA where to read and write.

🔹 Control Register Breakdown
(length & 0xFFF)


➡ DMA supports max 4095 transfers per block.

(2 << 18)  // 32-bit source width
(2 << 21)  // 32-bit destination width

Value	Width
0	8-bit
1	16-bit
2	32-bit
(1 << 26) | (1 << 27)


➡ Increment source and destination after each transfer.

🔹 Channel Enable
GPDMA_CH0_CFG = (1 << 0);


➡ Starts DMA transfer immediately.

🔹 Busy Status
return (GPDMA_INTSTAT & 0x01);


➡ Returns:

1 → DMA busy

0 → DMA done

4️⃣ How to Use in main()
int main(void)
{
    dma_init();

    dma_memcpy(src_buffer, dest_buffer, DMA_TRANSFER_SIZE);

    while (dma_is_busy());

    while (1);
}
*/
