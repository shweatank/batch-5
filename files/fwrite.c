#include"header.h"
void main()
{
int a[5]={10,20,30,40,50};
FILE*fp=fopen("data1","w");
fwrite(a,sizeof(int),5,fp);
fclose(fp);
}
