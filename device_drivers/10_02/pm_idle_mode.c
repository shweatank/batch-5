/*****************************************************
 * LPC21xx Power Management – Idle Mode Example
 * Bare-metal, no startup code shown
 *****************************************************/

#define PCON        (*(volatile unsigned long *)0xE01FC0C0)
#define VPBDIV      (*(volatile unsigned long *)0xE01FC100)

/* Timer0 Registers */
#define T0IR        (*(volatile unsigned long *)0xE0004000)
#define T0TCR       (*(volatile unsigned long *)0xE0004004)
#define T0PR        (*(volatile unsigned long *)0xE000400C)
#define T0MR0       (*(volatile unsigned long *)0xE0004018)
#define T0MCR       (*(volatile unsigned long *)0xE0004014)

/* VIC */
#define VICIntEnable (*(volatile unsigned long *)0xFFFFF010)
#define VICVectAddr0 (*(volatile unsigned long *)0xFFFFF100)
#define VICVectCntl0 (*(volatile unsigned long *)0xFFFFF200)
#define VICVectAddr  (*(volatile unsigned long *)0xFFFFFF00)

void timer0_isr(void) __attribute__((interrupt("IRQ")));

/*---------------------------------------------------*/
void delay_timer_init(void)
{
    VPBDIV = 0x01;          // PCLK = CCLK

    T0PR  = 15000 - 1;      // Prescaler → 1 ms tick (for 15 MHz PCLK)
    T0MR0 = 1000;           // 1000 ms = 1 second
    T0MCR = (1 << 0) |      // Interrupt on MR0
            (1 << 1);       // Reset on MR0

    T0TCR = 0x01;           // Enable Timer0
}

/*---------------------------------------------------*/
void timer0_isr(void)
{
    T0IR = 1;               // Clear interrupt flag
    VICVectAddr = 0;        // End of interrupt
}

/*---------------------------------------------------*/
void enter_idle_mode(void)
{
    PCON = 0x01;            // Set IDLE bit

    /* ARM7 instruction to enter low power state */
    __asm volatile ("MCR p15, 0, r0, c7, c0, 4");
}

/*---------------------------------------------------*/
int main(void)
{
    /* Configure Timer0 Interrupt in VIC */
    VICVectAddr0 = (unsigned long)timer0_isr;
    VICVectCntl0 = (1 << 5) | 4;   // Enable slot, Timer0 IRQ = 4
    VICIntEnable = (1 << 4);       // Enable Timer0 interrupt

    delay_timer_init();

    while (1)
    {
        enter_idle_mode();  // CPU sleeps here

        /* CPU resumes here after interrupt */
        /* Do some work if needed */
    }
}
