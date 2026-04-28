
link_list * delete_nth(link_list *head)
{
	link_list *fast,*slow;
	int n;
	printf("enter the nth :");scanf("%d",&n);
	if(!head)
	{
		printf("list is empty\n");
		return head;
	}
	fast=head;
	slow=head;
	while(n)
	{
		fast=fast->link;
		if(fast==NULL)
		{
			printf("n is greater\n");
			return head;
		}
		n--;
	}
	if(fast==NULL)
	{
		link_list *temp=head;
		head=head->link;
		free(head);
		return head;
	}
	while(fast->link!=NULL)
	{
		slow=slow->link;
		fast=fast->link;
	}
	link_list *temp=slow->link;
	slow->link=slow->link->link;
	free(temp);
	return head;
}
