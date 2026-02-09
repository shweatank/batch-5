volatile unsigned int *GPFSEL1  = (unsigned int *)(0xFE200000 + 0x04); 
volatile unsigned int *GPSET0   = (unsigned int *)(0xFE200000 + 0x1C); 
volatile unsigned int *GPCLR0   = (unsigned int *)(0xFE200000 + 0x28); 

int main(void)
{
    volatile unsigned int i;

    
    *GPFSEL1 |= (1 << 21);

    while(1)
    {
       
        *GPSET0 = (1 << 17);

        
        for(i = 0; i < 1000000; i++);

       
        *GPCLR0 = (1 << 17);

        
        for(i = 0; i < 1000000; i++);
    }
}

