#include"header.h"
FILE *fp;
struct stu
{
char name[20];
int age;
char loc[30];
char prof[30];
struct stu *next;
};
struct stu *head=NULL;
long int flag;
void main()
{
int ret;
struct stu *last;
printf("%s\t%5s%20s%20s\n","name","age","location","profession");
while(1)
{
fp=fopen("data.txt","r+");
if(fp==0)
{
printf("no file\n");
exit(0);
}
struct stu *new=calloc(1,sizeof(struct stu));
while(fscanf(fp,"%s%d%s%s",new->name,&new->age,new->loc,new->prof)!=-1)
{
printf("%s\t%5d%20s%20s\n",new->name,new->age,new->loc,new->prof);
if(head==0)
head=new;
else
{
last=head;
while(last->next)
last=last->next;
last->next=new;
}
}
flag=ftell(fp);
fclose(fp);
}
}
