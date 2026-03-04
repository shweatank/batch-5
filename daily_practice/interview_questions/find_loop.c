#include<stdio.h>
#include<stdbool.h>

typedef struct node
{
	int data;
	struct node *next;
}node;

bool FindLoop(node *head)
{
	node *slow,*fast;
	slow=fast=head;
	while(fast && fast->next)
	{
		slow=slow->next;
		fast=fast->next->next;
		if(slow==fast)
			return true;
	}
	return false;
}
int main()
{
	node *head=NULL;
	if(FindLoop(head))
		printf("loop is found\n");
	else
		printf("loop is not found\n");
}
