#include<stdio.h>
int main()
{
   int i,j,k,temp;
   int num;
   printf("enter the num\n");
   scanf("%d",&num);
   printf("octal=%o hex=%x\n",num,num);
   int pos;
/*  for(pos=31;pos>=0;pos--)
 {
    printf("%d",(num>>pos)&1);
    if(pos%8==0)
    printf(" ");
 }
 printf("\n");
 */
 int r;
 int s[20];
 i=0;
 for(temp=num;temp>=8; )
 {
    r=temp%8;
    if(r>0)
   {
     s[i]=r;
   }
   else
  {
    s[i]=r;
  }
  i++;
  temp=temp/8;
 }
 r=temp%8;
// printf("%d\n",r);
 if(r<8)
 {
    s[i]=r;
    i++;
 }
  else
 {
   s[i]=r;
   i++;
 }
 j=i-1;
 for(i=j;i>=0;i--)
	 printf("%d",s[i]);
  printf("\n");
 int temp1;
 char p[20];
 i=0;
 for(temp1=num;temp1>=16;  )
 {
	 r=temp1%16;
	 if(r>9)
	 {
		 p[i]=(r-10)+'A';
	 }
	 else
	 {
		 p[i]=r+48;
	 }
	 i++;    
        temp1=temp1/16;  
 }
 r=temp1%16;
 if(r>9)
 {
	 p[i]=(r-10)+'A';
	 i++;
 }
 else
 {
	 p[i]=r+48;
	 i++;
 }
 j=i-1;
 for(i=j;i>=0;i--)
	 printf("%c",p[i]);
}                    
