#include"header.h"
void main()
{
int p[2];
pipe2(p,O_NONBLOCK);
if(fork())
{
	if(fork())
	{
char s[11]="jsai_kumar";
write(p[1],s,sizeof(s));
	}
	else
	{
          sleep(1);
	char s[6];
        read(p[0],s,5);
printf("%s\n",s);
	}
}
else
{
sleep(2);
char s[6];
read(p[0],s,5);
printf("%s\n",s);
}
}
