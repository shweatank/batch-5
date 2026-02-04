#include<stdio.h>
#include<stdlib.h>
#include<stdio_ext.h>

typedef struct list
{
	int data;
	struct list *next;
}l;

l* add_node(l* h)
{
	l* new=NULL,*head=h;
	new=(l*)malloc(sizeof(l));
	if(!new)
	{
		printf("memory allocation failure\n");
		return head;
	}
	printf("enter data : ");
	scanf("%d",&new->data);
	if(head==NULL)
	{
		head=new;
		return head;
	}

	while(head->next)
		head=head->next;
	head->next=new;
	new->next=NULL;
	return h;
}

void print(l* head)
{
	while(head)
	{
		printf("%d ",head->data);
		head=head->next;
	}
	printf("\n");
}

l* rev_nodes(l* head)
{
	l*prev=NULL,*nxt=NULL,*cur=head;
	while(cur)
	{
		nxt=cur->next;
		cur->next=prev;
		prev=cur;
		cur=nxt;
	}
	return prev;
}

l* remv_kth_node(l* h)
{
	l* prev,*head=h;
	int pos;
	printf("enter kth pos : ");
	scanf("%d",&pos);
	--pos;
	head=rev_nodes(head);
	while(head)
	{
		prev=head;
		if(pos==0)
		{
			prev->next=head->next;
			free(head);
			return h;
		}
		head=head->next;
	}

	return rev_nodes(h);
}

int main()
{
	l *head=NULL;
	char ch;
	while(1)
	{
		printf("enter choice :1.add 2.print 3.rem kth node 4.exit\n");
		__fpurge(stdin);
		scanf("%c",&ch);
		switch(ch)
		{
			case '1': head=add_node(head);
				  break;
			case '2': print(head);
				  break;
			case '3': head=remv_kth_node(head);
				  break;
			case '4': exit(0);
		}
	}
}
