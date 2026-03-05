#include<stdio.h>
#include<stdlib.h>
typedef struct st
{
	int rollno;
	char name[20];
	float marks;
	struct st *next;
}sll;


void add_begin(sll **);
void print_node(sll *);
void add_middle(sll **);
int count_node(sll *);

void main()
{
	int op,c=0;
	sll *hptr=0;
	while(1)
	{
		printf("1.add_begin\n 2.print_nodes\n 3.add_middle\n");
		printf("enter the op\n");
		scanf("%d",&op);

		switch(op)
		{

			case 1: add_begin(&hptr);
				break;

			case 2: print_node(hptr);
				break;
			case 3: add_middle(&hptr);
				break;
			case 4:c=count_node(hptr);
				printf("no of nodes=%d\n",c);
                                break;
		}
	}
}
void add_begin(sll **hptr)
{
	sll *new=malloc(sizeof(sll));
	printf("enter the rno name and marks\n");
	scanf("%d%s%f",&new->rollno, new->name, &new->marks);
	new->next=*hptr;
	*hptr=new;
}

void print_node(sll *hptr)
{
	if(hptr==0)
	{
		printf("no records\n");
		return;
	}
	while(hptr)
	{
		printf("%d %s %f\n",hptr->rollno,hptr->name,hptr->marks);
		hptr=hptr->next;

	}
}

void add_middle(sll **hptr)
{
sll *new, *pos;
new=malloc(sizeof(sll));
printf("enter the rnno name and marks\n");
scanf("%d%s%f",&new->rollno, new->name, &new->marks);

if(*hptr==0 || (new->rollno < (*hptr)->rollno))
{
new->next=*hptr;
*hptr=new;
}
else
{
pos=*hptr;
while(pos->next!=0 && (new->rollno > (pos->next->rollno)))
pos=pos->next;
new->next=pos->next;
pos->next=new;
}
}

int count_node(sll *hptr)
{
int c=0;
if(hptr==0)
printf("no records found\n");

while(hptr)
{
c++;
hptr=hptr->next;
}
return c;
}
