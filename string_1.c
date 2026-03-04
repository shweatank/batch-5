/*Exercise 5-3. Write a pointer version of the function strcat that we showed in Chapter 2:
strcat(s,t) copies the string t to the end of s.*/





 #include<stdio.h>
char *my_strcat(char *p,char *q)
{
	char *res=p;

	while(*++p);
	while(*p++=*q++);

	return res;
}
int main()
{
	char p[100]="gurunath",q[]="gurunath";

       printf("%s\n",my_strcat(p,q));
}

