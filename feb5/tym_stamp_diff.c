#include<stdio.h>

struct tym
{
	unsigned int hr:5;
	unsigned int min:6;
	unsigned int sec:6;
};

int main()
{
	struct tym t1={1,1,1},t2={2,2,2},t3;
	int sec=(((t1.hr*3600)+(t1.min*60)+(t1.sec))-((t2.hr*3600)+(t2.min*60)+t2.sec));
	if(sec<0)
		sec=-sec;
	printf("total secs differ : %d\n",sec);
	t3.hr=sec/3600;
	t3.min=(sec%3600)/60;
	t3.sec=sec%60;
	printf("time difference :%d : %d :%d \n",t3.hr,t3.min,t3.sec);
}
