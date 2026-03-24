#include"header.h"
typedef struct st
{
int val;
struct st *next;
}sll;
sll *head=NULL,*tail=NULL;
void main()
{
int op;
while(1)
{
printf("enter the option:\n");
scanf("%d",&op);
switch(op)
{
case 1:
add_end();
break;
case 2:
print();
break;
case 3:
insert_middle();
break;
}
}

}
void add_middle()
{
sll *new=calloc(1,sizeof(*new));
printf("enter the number\n");
scanf("%d",&new->val);
if(head==0||new->val < head->val)
{
new->next=head;
head=new;
}
else
{
sll *pos=head;
while(pos->next && new->val > pos->next->val)
pos=pos->next;
new->next=pos->next;
pos->next=new;
}
}
void print()
{

sll *temp=head;
while(temp)
{
printf("%d ",temp->val);
temp=temp->next;
}
printf("\n");
}
void insert_middle()
{
sll *fast,*slow;
fast=slow=head;
while(fast->next&&fast->next->next)
{
slow=slow->next;
fast=fast->next->next;
}
sll *new=calloc(1,sizeof(sll));
new->val=10;
new->next=slow->next;
slow->next=new;
}
void add_end()
{
sll *new=calloc(1,sizeof(sll));
printf("enter value:\n");
scanf("%d",&new->val);
if(head==0)
head=tail=new;
else
tail=tail->next=new;
}
