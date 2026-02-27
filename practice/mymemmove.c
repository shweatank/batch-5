#include<stdio.h>
#include<string.h>
#include<stdlib.h>
void *my_memmove(void *dest,const void *src,size_t n)
{
	char *d=(char *)dest;
	const char *s=(const char *)src;
	if(d<s)
	{
		for(size_t i=0;i<n;i++)
			d[i]=s[i];
	}
	else
	{
		for(size_t i=n;i>0;i--)
			d[i-1]=s[i-1];
	}
	return dest;
}
void *my_memcpy(void *dest,const void *src,size_t n)
{
	char *d=(char *)dest;
	const char *s=(const char *)src;
	for(size_t i=0;i<n;i++)
		d[i]=s[i];
	return dest;
}
int main()
{
	char str[30],ptr[30];
	fgets(str,30,stdin);if(str[strlen(str)-1]==10){str[strlen(str)-1]=0;}
	puts(str);
	strcpy(ptr,str);
	my_memmove(str+2,str,6);
	puts(str);
	printf("now lets see memcpy\n");
	puts(ptr);
	my_memcpy(ptr+2,ptr,6);
	puts(ptr);
return 0;
}
