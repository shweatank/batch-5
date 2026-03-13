#include<stdio.h>
#include<stdlib.h>

typedef struct Node{
	int n;
	struct Node*next;
}node;

/* Adding Node at Beginning of the single linked list */
node *Adding_first(node *head)
{
	node *newnode=NULL;
	newnode=calloc(1,sizeof(node));
	puts("enter the value");
	scanf("%d",&newnode->n);
	if(head==NULL)
	{
		head=newnode;
	}
	else
	{
		newnode->next=head;
		head=newnode;
	}
	return head;
}

/* Adding Node at End of the single linked list */
node *Adding_last(node *head)
{
	node *newnode=NULL,*temp=NULL;
	newnode=calloc(1,sizeof(node));
	puts("enter the value");
	scanf("%d",&newnode->n);
	if(head==NULL)
	{
		head=newnode;
	}
	else
	{
		temp=head;
		while(temp->next)
		{
			temp=temp->next;
		}
		temp->next=newnode;
	}
	return head;
}


/* Adding the node at the Middle of the single linked list */
node *Adding_at_middle(node *head)
{
	node *newnode=NULL,*fast=NULL,*slow=NULL;
	newnode=calloc(1,sizeof(node));
	puts("enter the value");
	scanf("%d",&newnode->n);
	if(head==NULL)
	{
		head=newnode;
		return head;
	}
	fast=slow=head;
	while(fast->next && fast->next->next)
	{
		slow=slow->next;
		fast=fast->next->next;
	}
	newnode->next=slow->next;
	slow->next=newnode;
	return head;
}

/* Adding the node from last at the specific position in single linked list */
node *Adding_at_specific_last(node *head)
{
	node* newnode=NULL,*slow=NULL,*fast=NULL;
	newnode=calloc(1,sizeof(node));
	int n;
	puts("enter the position");
	scanf("%d",&n);
	puts("enter the value");
	scanf("%d",&newnode->n);
	if(head==NULL && n!=1)
	{
		puts("list is empty");
		return head;
	}
	fast=slow=head;
	for(int i=0;i<n;i++)
	{
		if(fast==NULL)
		{
			printf("position is greater than the length\n");
			return head;
		}
		fast=fast->next;
	}
	if(fast==NULL)
	{
		newnode->next=head;
		head=newnode;
		return head;
	}
	while(fast->next)
	{
		slow=slow->next;
		fast=fast->next;
	}
	newnode->next=slow->next;
	slow->next=newnode;
	return head;
}

/* Adding the node at the specific position in single linked list from the first */
node *Adding_at_specific_first(node *head)
{
	node *newnode=NULL,*temp=NULL;
	int n;
	puts("enter the position number");
	scanf("%d",&n);
	newnode=calloc(1,sizeof(node));
	puts("enter the value");
	scanf("%d",&newnode->n);
	if(head==NULL&&n!=1)
	{
		puts("list is empty");
		return head;
	}
	if(n==1)
	{
		newnode->next=head;
		head=newnode;
		return head;
	}
	temp=head;
	for(int i=1;i<n-1;i++)
	{
		if(temp==NULL)
		{
			puts("position is greater than length\n");
			return head;
		}
		temp=temp->next;
	}
	newnode->next=temp->next;
	temp->next=newnode;
	return head;
}

/* Printing all details of all nodes in a single linked list */
void Print(node *head)
{
	while(head)
	{
		printf("%d ",head->n);
		head=head->next;
	}
	printf("\n");
}


/* Reversing the node in a single lis */
node *Reverse_list(node *head)
{
	node *prev=NULL,*cur=NULL,*next=NULL;
	cur=head;
	if(head==NULL)
	{
		puts("no nodes are there to reverse list is empty");
		return head;
	}
	while(cur)
	{
		next=cur->next;
		cur->next=prev;
		prev=cur;
		cur=next;
	}
	return prev;
}

/* Deleting the node from first in single linked list */

node *Delete_first(node *head)
{
	if(head==NULL)
	{
		puts("list is empty no nodes to delete");
		return head;
	}
	node *temp=NULL;
	temp=head;
	head=head->next;
	free(temp);
	temp=NULL;
	return head;
}

/* Deleting the node from the last in single linked list */
node *Delete_last(node *head)
{
	node*temp=head,*prev=temp;
	if(head==NULL)
	{
		puts("list is empty no nodes to delete");
		return head;
	}
	if(head->next==NULL)
	{
		free(head);
		head=NULL;
		return head;
	}
	while(temp->next)
	{
		prev=temp;
		temp=temp->next;
	}
	prev->next=NULL;
	free(temp);
	temp=NULL;
	return head;
}

/* Deleting the node at middle of the single linked list */
node *Delete_at_middle(node*head)
{
	if(head==NULL)
	{
		puts("no nodes to delete in list");
		return head;
	}
	if(head->next==NULL)
	{
		free(head);
		head=NULL;
		return head;
	}
	node*slow,*fast,*temp=NULL;
	slow=fast=head;
	while(fast && fast->next)
	{
		temp=slow;
		slow=slow->next;
		fast=fast->next->next;
	}
	temp->next=slow->next;
	free(slow);
	return head;
}


/* Deleting the node at specific position from the last in single linked list */
node* Delete_at_specific_last(node * head)
{
	if(head==NULL)
	{
		puts("no nodes to delete in the list");
		return head;
	}
	int n;
	printf("enter the position to delete\n");
	scanf("%d",&n);
	node *slow=head,*fast=head,*temp=NULL;
	for(int i=0;i<n;i++)
	{
		if(fast==NULL)
		{
			printf("position is greater than the length\n");
			return head;
		}
		fast=fast->next;
	}
	if(fast==NULL)
	{
		temp=head;
		head=head->next;
		free(temp);
		return head;
	}
	while(fast->next)
	{
		slow=slow->next;
		fast=fast->next;
	}
	temp=slow->next;
	slow->next=slow->next->next;
	free(temp);
	return head;
}


/* Deleting the specific position from the first in single linked list */
node* Delete_at_specific_first(node *head)
{
	if(head==NULL)
	{
		puts("no node in the list to delete");
		return head;
	}
	int n;
	printf("enter the position\n");
	scanf("%d",&n);
	node *temp=head,*temp2=NULL;
	if(n==1)
	{
		temp2=head;
		head=head->next;
		free(temp2);
		return head;
	}
	for(int i=1;i<n-1;i++)
	{
		if(temp==NULL)
		{
			puts("position is greater than the length of the list");
			return head;
		}
		temp=temp->next;
	}
	temp2=temp->next;
	temp->next=temp->next->next;
	free(temp2);
	return head;
}


/* Delete the full single list and free the memory */
node *Delete_full_list(node *head)
{
	if(head==NULL)
	{
		printf("already list is empty no node to delete and free the memory\n");
		return head;
	}
	node *temp=NULL;
	while(head)
	{
		temp=head;
		head=head->next;
		free(temp);
	}
	return head;
}

/* Finding the length of the single linked list using recurrsion */
int Length_list(node *head)
{
	static int n;
	if(head==NULL)
	{
		int x=n;
		n=0;
		return x;
	}
	else
	{
		n++;
		head=head->next;
		Length_list(head);
	}
}

/* Sorting the single linked list */
node *Sort_List(node *head)
{
	if(head==NULL)
	{
		puts("list is empty");
		return head;
	}
	node *temp1,*temp2=NULL;
	int f=0;
	while(1)
	{
		f=0;
		temp1=head;
		while(temp1->next != temp2)
		{
			if(temp1->n > temp1->next->n)
			{
				int n=temp1->n;
				temp1->n=temp1->next->n;
				temp1->next->n=n;
				f=1;
			}
			temp1=temp1->next;
		}
		temp2=temp1;
		if(f==0)
			break;
	}
	return head;
}

/* Checking the single linked list is palindrome or not a palindrome */
int Palindrome_List(node *head)
{
	if(head==NULL||head->next==NULL)
		return 1;
	node *slow,*fast;
	slow=fast=head;
	while(fast && fast->next)
	{
		slow=slow->next;
		fast=fast->next->next;
	}
	if(fast)
		slow=slow->next;
	node *half,*first=head,*temp;
	half=Reverse_list(slow);
	temp=half;
	while(temp)
	{
		if(temp->n != first->n)
			return 0;
		first=first->next;
		temp=temp->next;
	}
	Reverse_list(half);
	return 1;
}
int main()
{
	node *head=NULL;
	char ch,ch2;
	while(1)
	{
		puts("a: adding node\nd: deleting node\nr: reversing node\nl: length of list\ns: sorting the list\nc:Check for Palindrome\np: printing node\ne: exit");
		scanf(" %c",&ch);
		switch(ch)
		{
			case 'a':puts("1: adding first\n2: adding last\n3: adding at middle\n4: adding from last specific pos\n5: adding at specific from first");
				 scanf(" %c",&ch2);
				 switch(ch2)
				 {
					 case '1':head=Adding_first(head);
						  break;
					 case '2':head=Adding_last(head);
						  break;
					 case '3':head=Adding_at_middle(head);
						  break;
					 case '4':head=Adding_at_specific_last(head);
						  break;
					 case '5':head=Adding_at_specific_first(head);
						  break;
				 }
				 break;
			case 'd':puts("1: delete first\n2: delete last\n3: delete at middle\n4: delete at specific from last\n5: delte from first at specific pos\n6: Delete the full list");
				 scanf(" %c",&ch2);
				 switch(ch2)
				 {
					 case '1':head=Delete_first(head);
						  break;
					 case '2':head=Delete_last(head);
						  break;
					 case '3':head=Delete_at_middle(head);
						  break;
					 case '4':head=Delete_at_specific_last(head);
						  break;
					 case '5':head=Delete_at_specific_first(head);
						  break;
					 case '6':head=Delete_full_list(head);
						  break;
				 }
				 break;
			case 'r':head=Reverse_list(head);
				 break;
			case 'p':Print(head);
				 break;
			case 'l':int n=Length_list(head);
				 printf("length of the list is %d\n",n);
				 break;
			case 's':head=Sort_List(head);
				 break;
			case 'c':if(Palindrome_List(head))
					 printf("list is palindrome\n");
				 else
					 printf("list is not palindrome\n");
				 break;
			case 'e':exit(0);
		}
	}
}
