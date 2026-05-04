#include<stdio.h>
#include<stdlib.h>
void main()
{
char s[10];
printf("enter the date\n");
scanf("%s",s);

char *p=s;
int day=0,month=0,year=0;

day=(*p++ - '0');
day=day*10+(*p++ -'0');
printf("%d\n",day);
if(day<=0||day>30)
{
printf("invalid date\n");
exit(0);
}
p++;

month=(*p++-'0');
month=month*10+(*p++-'0');
printf("%d\n",month);
if(month<=0||month>12)
{
printf("invalid date\n");
exit(0);
}
p++;

while(*p)
year=year*10+(*p++-'0');
printf("%d\n",year);
if(year<=0 || year>2026)
{
printf("invalid date\n");
exit(0);
}
 

//printf("day=%d\n",day);

}
