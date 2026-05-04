#include<stdio.h>
#include<stdlib.h>
typedef struct st
{
int data;
struct st *next;
}sll;

void add_begin(sll **);
void add_end(sll **);
void print_data(sll *);
int count_nodes(sll *);
int * find_middle_node(sll *);
void del_f_node(sll **);
void add_node_at_middle_in_sorted_link(sll *);
void rev_link(sll **);
void del_all_nodes(sll **);
void del_last(sll *);
void main()
{
int op;
sll * hptr=0;
while(1)
{
printf(" 1.add begin\n 2.print data \n 3.add end\n 4.count nodes\n 5.find middle node\n 6.delete first node\n 7.add_node_at_middle_in_sorted_link\n 8.reverse the linkeed list\n 9.delete all nodes\n 10.delete last node\n");
printf("enter the op\n ");
scanf("%d",&op);
switch(op)
{
case 1:add_begin(&hptr);
break;
case 2:print_data(hptr);
break;
case 3:add_end(&hptr);
break;
case 4:int r=count_nodes(hptr);
printf("no of nodes=%d\n",r);
break;
case 5:int *p=find_middle_node(hptr);
printf("%d\n",*p);
break;
case 6:del_f_node(&hptr);
break;
case 7:add_node_at_middle_in_sorted_link(hptr);
break;
case 8:rev_link(&hptr);
break;
case 9:del_all_nodes(&hptr);
break;
case 10:del_last(hptr);
break;
}
}
}
void add_begin(sll **hptr)
{
sll * new=malloc(sizeof(sll));

printf("enter the data\n");
scanf("%d",&new->data);

if(*hptr==0)
{
new->next=*hptr;
*hptr=new;
}
else
{
new->next=*hptr;
*hptr=new;
}
}

void print_data(sll *hptr)
{
if(hptr==0)
printf("no records found\n");
while(hptr)
{
printf("%d\n",hptr->data);
hptr=hptr->next;
}
}

void add_end(sll **hptr)
{
sll *last;
sll *new=malloc(sizeof(sll));
printf("enter the data\n");
scanf("%d",&new->data);

if(*hptr==0)
{
new->next=*hptr;
*hptr=new;
}
else
{
last=*hptr;
while(last->next)
last=last->next;
last->next=new;
new->next=0;
}
}

int count_nodes(sll *hptr)
{
	int c=0;
if(hptr==0)
printf("no records\n");

while(hptr)
{
hptr=hptr->next;
c++;
}
return c;
}

int * find_middle_node(sll *hptr)
{
sll *f=hptr;
sll *s=hptr;

if(hptr==0)
printf("no records found\n");

if(f->next  && f->next->next)
{
f=f->next->next;
s=s->next;
}
return (int *)s;
}

void add_node_at_middle_in_sorted_link(sll *hptr)
{
sll *f=hptr;
sll *s=hptr;
if(hptr==0)
printf("no record\n");

sll *new=malloc(sizeof(sll));
scanf("%d",&new->data);
while(f->next && f->next->next)
{
f=f->next->next;
s=s->next;
}
new->next=s->next;
s->next=new;
}

void rev_link(sll **hptr)
{
sll *p=0;
sll *next=0;
sll *c=*hptr;

while(c)
{
next=c->next;
c->next=p;
p=c;
c=next;
}
*hptr=p;
}

void del_f_node(sll ** hptr)
{
if(*hptr==0)
printf("no records\n");
else
{
sll *temp=*hptr;
*hptr=temp->next;
free(temp);
}
}

void del_all_nodes(sll **hptr)
{
if(*hptr==0)
printf("no nodes are present\n");
sll *temp;

while(*hptr)
{
temp=*hptr;
*hptr=(*hptr)->next;
free(temp);
}
}

void del_last(sll *hptr)
{
if(hptr==0)
printf("no nodes are present\n");
sll *del=hptr;
while(del->next->next)
del=del->next;
free(del->next);
del->next=0;
}

