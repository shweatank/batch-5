#include<stdio.h>
#include<stdlib.h>

struct st{
        char name[100];
        int age;
        char loc[100];
        char prof[100];
}s;

int main(){
        /*char name[100];
        int age;
        char loc[100];
        char prof[100];*/
        FILE *fp;
        fp=fopen("file7","r");
        if(fp==NULL){
                printf("file open:error\n");
                exit(0);
        }
        while(1){
                while(fscanf(fp,"%s %d %s %s",s.name,&s.age,s.loc,s.prof)==4){
                        printf("%s %d %s %s\n",s.name,s.age,s.loc,s.prof);
                }
        }
        fclose(fp);
}
