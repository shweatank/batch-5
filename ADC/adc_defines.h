//adc_defines.h

//clock defines
#define FOSC 12000000
#define CCLK (FOSC*5)
#define PCLK (CCLK/4)
#define ADCCLK 3000000 
#define CLKDIV ((PCLK/ADCCLK)-1)

//ADCR sfr defines
#define CLKDIV_BITS    8
#define PDN_BIT       21
#define ADC_START_BIT 24

//ADDR sfr defines
#define RESULT_BITS 6
#define DONE_BIT    31

//User defined datatypes
typedef unsigned int u32
typedef float f32
