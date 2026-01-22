#include<stdio.h>
#include<math.h>
int isprime(int );
int main()
{
        int n;
        puts("enter the number");
        scanf("%d",&n);
        if(isprime(n))
                puts("it is prime number");
        else
                puts("it is not a prime number");
        return 0;
}
int isprime(int n)
{
        int s=sqrt(n);
        if(n<2)
                return 0;
        if(n==2)
                return 1;
        for(int i=2;i<=s;i++)
        {
                if(n%i==0)
                        return 0;
        }
        return 1;
}
