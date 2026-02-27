#include<stdio.h>
#include<string.h>
#include<stdlib.h>
float add(float a,float b)
{
	return a+b;
}
float sub(float a,float b)
{
        return a-b;
}
float mul(float a,float b)
{
        return a*b;
}
float division(float a,float b)
{
        return a/b;
}
int main()
{
	float (*fptr[4])(float,float)={add,sub,mul,division};
	float res,x,y;
	int opt;
	while(1)
	{
		printf("enter your choice:0-add,1-sub,2-mul,3-div,4-exit\n");scanf("%d",&opt);
		if(opt==4){exit(0);}
		printf("enter  numbers:");scanf("%f%f",&x,&y);
		res=fptr[opt](x,y);
		printf("result=%0.2f\n",res);
	}


}
