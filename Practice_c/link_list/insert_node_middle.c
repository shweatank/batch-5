void insert(link_list *head)
{
	link_list *slow,*fast,*prev,*new;
	fast=head;
	slow=head;
	while(fast && fast->link)
	{
		prev=slow;
		fast=fast->link->link;
		slow=slow->link;
	}
	new=malloc(sizeof(link_list));
	new-val=10;
	prev->link=new;
	new->link=slow;
}

