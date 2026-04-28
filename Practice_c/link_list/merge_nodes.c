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
        for(int i=0;i<5;i++)
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
/*
link_list *merge_list(link_list *head1,link_list *head2)
{
	link_list *prev=NULL,*temp=NULL,*temp2;
	if(head1==NULL)
		return head2;
	if(head2==NULL)
		return head1;
	if(head1->val > head2->val)
	{
		temp=head2->link;
		head2->link=head1;
		head1=head2;
		prev=head1;
		head2=temp;
		temp=head1->link;
	        while(temp && head2)
	        {
			if(temp->val >head2->val)
			{
				prev->link=head2;
				temp2=head2->link;
				head2->link=temp;
				head2=temp2;
			}
			else
			{
				temp=temp->link;
			}
		}
	}
	return head1;
}
*/
link_list *merge_list(link_list *head1,link_list *head2)
{
	link_list *prev,*head,*temp;
	if(head1->val > head2->val)
	{
		temp=head1;
		head1=head2;
		head2=temp;
	}
	head=head1;
	while(head1 && head2)
	{
		prev=NULL;
		while(head1 && head1->val<=head2->val)
		{
			prev=head1;
			head1=head1->link;
		}
		prev->link=head2;

		temp=head1;
		head1=head2;
		head2=temp;
	}
	return head;
}
int main()
{
	link_list *head1=NULL,*head2=NULL,*merge;
	head1=creat_list(head1);
	printf("enter the next list\n");
	head2=creat_list(head2);
	printf("......list 1.......\n");
	print_list(head1);
	printf("......list 2.......\n");
	print_list(head2);
	merge=merge_list(head1,head2);
	printf("after merge\n");
	print_list(merge);
}
