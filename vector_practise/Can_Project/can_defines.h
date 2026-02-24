#ifndef __CAN_DEFINES_H__
#define __CAN_DEFINES_H__

// Bit timing for CAN1: ~125 kbps @ PCLK = 60 MHz (example)
#define PCLK        60000000UL   // Hz
#define BIT_RATE    125000UL     // Hz
#define QUANTA      16
#define BRP         (PCLK / (BIT_RATE * QUANTA))

#define SAMPLE_POINT   (0.7 * QUANTA)
#define TSEG1          ((int)SAMPLE_POINT - 1)   // prop_seg + phase_seg1
#define TSEG2          (QUANTA - (1 + TSEG1))    // phase_seg2
#define SJW            ((TSEG2 >= 5) ? 4 : (TSEG2 - 1))
#define SAM            0   // 0 or 1, sample 1 or 3 times

#define BTR_LVAL       ( (SAM << 23)              | \
                         ((TSEG2 - 1) << 20)      | \
                         ((TSEG1 - 1) << 16)      | \
                         ((SJW - 1)  << 14)       | \
                         ((BRP - 1) ) )

// C1CMR control bits
#define TR_BIT_SET     (1U << 0)   // Transmission request
#define RRB_BIT_SET    (1U << 2)   // Release receive buffer
#define STB1_BIT_SET   (1U << 5)   // Select transmit buffer 1

// C1GSR status bits
#define RBS_BIT_READ   (1U << 0)   // Receive buffer status
#define TBS1_BIT_READ  (1U << 2)   // Transmit buffer 1 status
#define TCS1_BIT_READ  (1U << 3)   // Transmission complete status

#endif
