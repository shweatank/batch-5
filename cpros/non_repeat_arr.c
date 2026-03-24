#include<stdio.h>
void main()
{
int a[7]={10,34,10,43,65,43,10};
int freq[100]={0};
for(int i=0;i<7;i++)
{
freq[a[i]]++;
if(freq[a[i]]>1)
printf("repeat=%d\n",a[i]);
}

/*for(int i=0;i<6;i++)
{
if(freq[a[i]]==1)
printf("non repeat=%d\n",a[i]);
}*/

}
