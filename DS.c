#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

typedef struct employee
{
	char name[50];
	int emp_id;
	float salary;
	struct employee *next;
}emp;

void create_node_begin(emp**);
void print_node(emp*);
void add_node_end(emp**);
void insert_at_position(emp**);
void delete_all_nodes(emp**);
void delete_first_node(emp**);
void delete_last_node(emp**);

void main()
{
	emp *hptr=0;
	int op;
	while(1)
	{
	printf("Enter the option:\n");
	printf("Enter 1)create_node 2)print_node 3)add_node_end 4)insert_at_position 5)delete_all_node 6)delete_first_node 7)delete_last_node 8)exit\n");
	scanf("%d",&op);
	switch(op)
	{
		case 1: create_node_begin(&hptr);
			break;
		case 2: print_node(hptr);
			break;
		case 3: add_node_end(&hptr);
			break;
		case 4: insert_at_position(&hptr);
			break;
		case 5: delete_all_nodes(&hptr);
			break;
		case 6: delete_first_node(&hptr);
			break;
		case 7: delete_last_node(&hptr);
			break;
		case 8: exit(0);
		default: printf("Unknown option\n");
	}
	}
}

void create_node_begin(emp **ptr)
{
	emp *new=malloc(sizeof(struct employee));
	printf("Enter the name, employee id and salary:\n");
	scanf("%s%d%f",new->name,&new->emp_id,&new->salary);
	new->next=*ptr;
	*ptr=new;
}

void print_node(emp *ptr)
{
	if(ptr==0)
	{
		printf("No records found:\n");
		return;
	}

	while(ptr)
	{
		printf("Name=%s emp_id=%d salary=%f\n",ptr->name,ptr->emp_id,ptr->salary);
		ptr=ptr->next;
	}
}

void add_node_end(emp **ptr)
{
	emp *new=malloc(sizeof(struct employee));
	printf("Enter the name, id and salary:\n");
	scanf("%s%d%f",new->name,&new->emp_id,&new->salary);
	new->next=0;
	if(*ptr==0)
		*ptr=new;
	else
	{
		emp *last=*ptr;
		while(last->next)
			last=last->next;
		last->next=new;
	}
}

void insert_at_position(emp **ptr)
{	
	emp *new=malloc(sizeof(struct employee));
	int pos;
	printf("Enter the name,id and salary to be inserted:\n");
	scanf("%s%d%f",new->name,&new->emp_id,&new->salary);
	printf("Enter the pos where new node to be inserted:\n");
	scanf("%d",&pos);
	
	if(pos==1)
	{
		new->next=*ptr;
		*ptr=new;
	}

	emp *temp=*ptr;

	for(int i=1;i<pos-1;i++)
	{
		temp=temp->next;
	}

	new->next=temp->next;
	temp->next=new;
}

void delete_all_nodes(emp **ptr)
{
	int c=1;
	if(*ptr==0)
	{
		printf("No records found\n");
		return;
	}
	emp *del=*ptr;
	while(del)
	{
		*ptr=del->next;
		free(del);
		printf("Node deleted: %d\n",c++);
		sleep(1);
		del= *ptr;
	}
	printf("All nodes are deleted\n");
}

void delete_first_node(emp **ptr)
{
	if(*ptr==0)
	{
		printf("No records found\n");
		return;
	}

	emp *del=*ptr;
	if(del)
	{
		*ptr=del->next;
		free(del);
		printf("First node is deleted\n");
	}
}

void delete_last_node(emp **ptr)
{
	if(*ptr==0)
	{
		printf("No records found\n");
		return;
	}

	emp *del=*ptr;
	if(del->next==NULL)
	{
		free(del);
		return;
	}
	
	while(del->next->next)
	{
		del=del->next;
	}
	free(del->next);
	del->next=NULL;
}
