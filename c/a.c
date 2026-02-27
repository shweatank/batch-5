#include <stdio.h>
#include<stdlib.h>
#include<math.h>
int unique(int i)
{
    int t=i,d;
    d=t/10;
    t=t/10;
    while(t)
    {
        if(d==(t%10))
        {
            return 0;
        }
        t/=10;
    }
    return 1;

}
int countNumbersWithUniqueDigits(int n) {
    int cnt=0;
    int v=pow(10,n);
    for(int i=0;i<v;i++)
    {
        if(unique(i))
        {
            cnt++;
        }
    }
    return cnt;
    
}
int main()
{
    int cou=countNumbersWithUniqueDigits(2);
    printf("%d\n",cou);
    cou=countNumbersWithUniqueDigits(0);
    printf("%d\n",cou);

    return 0;
}

