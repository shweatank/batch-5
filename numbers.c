#include<stdio.h>
void print_numbers(int num)
{
   static int c=0;
    if(c > num)
    {
        c=0;
       return;
    }
    printf("%d ",c++);
    print_numbers(num);
}

void littele_big_enedian(int *num)
{
	char   *p=malloc(1),*q=malloc(1);


	p=num; q=num+2;

	for(p,q;p<q;p++,q--)
	{
		char temp;
		temp=*p;
		*p=*q;
		*q=temp;
	}

	//printf("%x %x\n",*p,*q);
}
void main()
{
//	print_numbers(20);

	unsigned  int num=0x12345678;

	short int *p=&num;
	short int *q=&num;

	 q=q+1;

	short int temp;
	 temp=*p;
	 *p=*q;
	 *q=temp;

	 printf("%x",num);

}
