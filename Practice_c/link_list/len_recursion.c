#include<stdio.h>
#include<stdlib.h>
typedef struct link_list
{
	int val;
	struct link_list *link;
}link1;


link1* creat_list(link1 *head)
{
	char ch;
	link1 *next,*temp;
	next=malloc(sizeof(link1));
	printf("enter the data:");
	scanf("%d",&next->val);
	head=next;
	for(int i=0;i<5;i++)
	{
		temp=malloc(sizeof(link1));
	    printf("enter the data:");
	    scanf("%d",&temp->val);
	    next->link=temp;
	    next=temp;
	}
	return head;
}
void print(link1 *head)
{
	while(head)
	{
		printf("data is %d\n",head->val);
		head=head->link;
	}
}
int len_recu(link1 *head)
{
	if(head==NULL)
		return 0;
	else
		return (1 + len_recu(head->link));
}
int main()
{
	link1 *head=NULL;
	head=creat_list(head);
	print(head);
	printf("len of the link list %d\n",len_recu(head));
}
