volatile unsigned int GPIO;

int main(void)
{
    volatile unsigned int i;  
    
    while (1)
    {
        GPIO = 1;

        
        for(i = 0; i < 100000; i++);

        GPIO = 0;

        
        for(i = 0; i < 100000; i++);
    }
}

