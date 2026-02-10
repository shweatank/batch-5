#include<stdio.h>
#include<string.h>
int power(int a,int b)
{
        int i,res,ptr;
        for(ptr=1,i=0;i<b;i++)
        {
                ptr*=a;
        }
        return ptr;
}
int main()
{

        char s[32];

        printf("enter the string:\n");

        scanf("%s",s);

       int  val=0,base=0,i,len;
        len=strlen(s);

         for(i=len-1;i>=0;i--,base++)
         {
                 if(s[i]=='1')
                 {

                         val+=power(2,base);
                 }
         }

        printf("decimal val=%d\n",val);
}

