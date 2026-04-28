#include<stdio.h>
#include<stdlib.h>
typedef struct link1
{
	int val;
	struct link1 *link;
}link_list;

link_list * creat_list(link_list *head)
{
	link_list *temp,*temp1;
	for(int i=0;i<4;i++)
	{
		temp=malloc(sizeof(link_list));
		printf("enter the value:");
		scanf("%d",&temp->val);
		if(head==NULL)
		{
			head=temp;
		}
		else
		{
			temp1=head;
			while(temp1->link)
			{
				temp1=temp1->link;
			}
			temp1->link=temp;
		}
	}
	return head;
}
void print_list(link_list *head)
{
	if(head->link==NULL)
	{
		printf("list is empty\n");
		return ;
	}
	while(head)
	{
		printf("val is %d\n",head->val);
		head=head->link;
	}
}
/*link_list *swap_node(link_list *head)
{
	int swap;
	link_list *temp;
	temp=head;
	while(temp && temp->link)
	{
		swap=temp->val;
		temp->val=temp->link->val;
		temp->link->val=swap;
		temp=temp->link->link;
	}
	return head;
}*/
link_list *swap_node(link_list *head)
{
	link_list *temp,*swap,*prev=NULL,*cur;
	if(head->link!=NULL)
	{
		temp=head;
		head=head->link;
		while(temp && temp->link)
		{
			cur=temp->link;
			swap=temp->link->link;
			temp->link->link=temp;
			temp->link=swap;
			if(prev)
		            prev->link=cur;  
                            prev=temp;			    
			temp=temp->link;
		}
	}
	return head;
}
int main()
{
	link_list *head=NULL;
	head=creat_list(head);
	print_list(head);
	head=swap_node(head);
	printf("after swap\n");
	print_list(head);
}
