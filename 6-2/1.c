#include<stdio.h>
#include<stdlib.h>
typedef struct st
{
int rno;
struct st *next;
}sll;
struct st*head=NULL;
void add(sll*);
void print(sll*);
void main()
{
int op;
while(1)
{
printf("1.add 2.print\n");
scanf("%d",&op);
switch(op)
{
case 1:
add(head);
break;
case 2:
print(head);
break;
}
}
}
void add(sll *ptr)
{
sll *new=calloc(1,sizeof(sll));
printf("enter number\n");
scanf("%d",&new->rno);
if(head==0 ||(new->rno)<(head->rno))
{
new->next=head;
head=new;
}
else
{
sll*pos=head;
while(pos->next && (new->rno)>(pos->next->rno))
pos=pos->next;
new->next=pos->next;
pos->next=new;
}
}
void print(sll*head)
{
if(head==0)
{
printf("no nodes\n");
return ;
}
else
{
printf("%d\n",head->rno);
if(head->next)
print(head->next);
}
}
