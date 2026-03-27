#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include <sys/stat.h>
void main()
{
	FILE *fp;
	char s[100],place[50],prof[20];
	int age,a=0;

    struct stat st;

    lstat("data",&st);

    printf("%ld\n",st.st_mtime);

    long int lt=st.st_mtime;

    long int ut=st.st_mtime;

	int i;
	while(1)
	{
		 
	       	 fp=fopen("data","r");
	       lt=st.st_mtime;
	       ut=st.st_mtime;

		while(1)
		{

                       fseek(fp,a,SEEK_SET);
			while((i=fscanf(fp,"%s%d%s%s",s,&age,place,prof))!=EOF)
			printf("%s %d %s %s\n",s,age,place,prof);

	         	if(i==-1)
			{
			        for(;lt==ut;ut=st.st_mtime)
				{
					lstat("data",&st);
					sleep(1);
				}

				a=ftell(fp);
				a--;
			       fclose(fp);

			       break;

			}

		}
	}
}

