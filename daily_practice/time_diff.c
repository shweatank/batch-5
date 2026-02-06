#include<stdio.h>
typedef struct
{
	int hr,min,sec;
}Time;
long Seconds(Time);
Time Convert(long);
void Input(Time *);
int main()
{
	Time t1,t2;
	puts("enter the time 1");
	Input(&t1);
	puts("enter the time 2");
	Input(&t2);
	long tsec1=Seconds(t1);
	long tsec2=Seconds(t2);
	long dsec=tsec1-tsec2;
	if(dsec<0)
		dsec=-dsec;
	t1=Convert(dsec);
	printf("hr:%d min: %d sec:%d",t1.hr,t1.min,t1.sec);
}
void Input(Time *t)
{
	puts("enter the hour");
	scanf("%d",&t->hr);
	puts("enter the min");
	scanf("%d",&t->min);
	puts("enter the sec");
	scanf("%d",&t->sec);
}
long Seconds(Time t)
{
	t.hr=t.hr*60*60;
	t.min=t.min*60;
	long res=t.hr+t.min+t.sec;
	return res;
}
Time Convert(long sec)
{
	Time t;
	t.hr=sec/3600;
	t.min=(sec%3600)/60;
	t.sec=(sec%60);
	return t;
}
	
