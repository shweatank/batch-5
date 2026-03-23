/*Write a program to count blanks, tabs, and newlines.*/
#include<stdio.h>
void main()
{
	int ch;
	int b=0,t=0,n=0;
	while((ch=getchar())!=EOF)
	{
		if(ch==' ')
			b++;
		else if(ch=='\t')
			t++;
		else if(ch=='\n')
		     n++;
	}
	printf("\nblanks count:%d\n",b);
	printf("tabs count:%d\n",t);
	printf("newlines count:%d\n",n);
}
