/*Find the Middle of a Linked List

Find the middle element in one traversal.

If even number of nodes, return the second middle. 

*/

#include<stdio.h>
#include<stdlib.h>
#include<stdio_ext.h>
struct list
{
	int data;
	struct list *link;
};

static int cnt;
struct list* add_first(struct list* head)
{
	struct list *new=NULL;
	new=malloc(sizeof(new));
	cnt++;
	if(!new)
	{
		printf("memory allocation failure\n");
		return head;
	}
	printf("enter data : ");
	scanf("%d",&new->data);
	if(!head)
	{
		head=new;
		return head;
	}
	new->link=head;
	head=new;
	return head;
}

void print(struct list *head)
{
	while(head)
	{
		printf("%d\t",head->data);
		head=head->link;
	}
	printf("\n");
}

void find_middle(struct list *head)
{
	int val=0;
	while(head)
	{
		if(val==(cnt/2))
		{
			printf("midddle : %d\n",head->data);
			break;
		}
		head=head->link;
		val++;
	}
}

int main()
{
	struct list *head=NULL;
	char choice;
	while(1)
	{
		printf("enter choice A:add first P:print M:find middle\n");
		__fpurge(stdin);
		scanf("%c",&choice);
		switch(choice)
		{
			case 'A':head=add_first(head);
				 break;
			case 'M':find_middle(head);
				 break;
			case 'P':print(head);
				 break;
		}
	}
}
