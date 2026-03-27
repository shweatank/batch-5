#include<stdio.h>
int set_bit(int num)
{
        int c=0;

        while(num)
        {
                c++;
                num=num&num-1;
        }
        return c;
}

void main()
{
   int num;
  scanf("%d",&num);

  printf("%d",set_bit(num));
}

