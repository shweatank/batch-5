/*page no 45 2-3 Write a function htoi(s), which converts a string of hexadecimal digits
(including an optional 0x or 0X) into its equivalent integer value. The allowable digits are 0
through 9, a through f, and A through F.*/

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
 
	int a[]={10,11,12,13,14,15};
        char s[10];

	int d[32],i,j=0;

	  printf("enter the hexdcimal format like ox123af\n");
	  scanf("%s",s);

	  int len=strlen(s);

	  for(i=len-1;i>1;i--)
	  {
		  if((s[i]>='0'&&s[i]<='9')||(s[i]>='a'&&s[i]<='f')||(s[i]>='A'&&s[i]<='F'))
		  {

			  if(s[i]>='0'&&s[i]<='9')
			  {
				  int dec=s[i]-'0';
				  for(int i=0;i<=3;i++)
				  {
					  d[j++]=dec>>i&1;
				  }
			  }
			  else
			  {
				  int dec=a[s[i]-'a'];

				  printf("%d\n",dec);

				  for(int i=0;i<=3;i++)
				  {
					  d[j++]=dec>>i&1;
				  }
			  }
		  }
		  else
		  {
			  printf("invalid input:\n");
			  return 0;
		  }
	  }

	  int val=0;
	 for(int pos=0;pos<=31;pos++)
	  {

		  if(d[pos])
		  {
			  val+=power(2,pos);
		  }
	  }

	 printf("decimal=%d\n",val);
}































