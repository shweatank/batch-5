#include"header.h"
void main()
{
FILE*fp=fopen("/dev/basic_char","r");
char s[10];
fread(s,10,1,fp);
fclose(fp);
}
