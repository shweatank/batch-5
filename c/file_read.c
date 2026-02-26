#include <stdio.h>
#include <unistd.h>   // for sleep()
#include <stdlib.h>   // for system()

int main()
{
    FILE *fp;
    char s[200];

    while (1)
    {
        fp = fopen("data.txt", "r");
        if (fp == NULL)
        {
            printf("File not found\n");
            return 1;
        }

        system("clear");   // clear screen to show fresh output

        printf("Reading file content:\n");

        while (fgets(s, sizeof(s), fp) != NULL)
        {
            printf("%s", s);
        }

        fclose(fp);

        sleep(2);   // refresh every 2 seconds
    }

    return 0;
}


/*#include<stdio.h>
void main()
{
	FILE *fp;
	while(1)
	{
	fp=fopen("data.txt","r+");
	if(fp==0)
	{
		printf("File not found\n");
		return;
	}
	char s[100];

	//if(fscanf(fp,"%s",s)!=EOF)
	while(fgets(s,sizeof(s),fp) != NULL)
	{
		printf("%s\n",s);
	}
	}
	fclose(fp);
}*/
