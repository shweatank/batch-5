volatile unsigned int GPIO;

int main(void)
{
    while (1)
    {
        GPIO = 1;   
        GPIO = 0;   
    }
}

