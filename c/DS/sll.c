#include<stdio.h>
#include<stdlib.h>

struct st
{
int data;
struct st *link;
};

count_node(struct st *);
print_data(struct st *);
//creating linked list
int main()
{
struct st *head=0;
head=malloc(sizeof(struct st));
int->data=10;
head->link=0;
//node 2
struct st *current=0;
current=malloc(sizeof(struct st));
current->link=0;
current->data=20;
head->link=current;
//node 3
current=malloc(sizeof(struct st));
current->link=0;
current->data=30;
head->link->link=current;

}
//counting nodes
void count_node(struc st *head)
{
int c=0;
if(head==NULL)
printf("no nodes are present\n");

struct st *ptr=0;
ptr=head;

while(ptr!=0)
{
c++;
ptr=ptr->link;
}
printf("no 0f nodes=%d\n",c);
}

//print data
void print_data(struct st *head)
{
if(head==NULL)
printf("no data is present\n");

struct st *ptr=0;
ptr=head;

while(ptr!=0)
{
printf("%d \n",ptr->data);
ptr=ptr->link;
}
}

void add_at_end(struct st **head)
{
struct st *new,*temp;
new=malloc(sizeof(struct st));
printf("enter the data\n");
scanf("%d",&new->data);
new->link=0;

if(*head==0)
printf("no record found\n");
*head=new;
else
{
temp=*head;
while(temp->link)
temp=temp->link;
temp->link=new;
}
}

void add_at_begin(struct st **head)
{
struct st *new;
new=malloc(sizeof(struct st));
printf("enter data\n");
scanf("%d",
new->link=*head;
*head=new;
}
//Deleting the first node
struct st * del_first(struct st **head)
{
if(*head==0)
printf("list is empty\n");
else
{
struct st *temp=*head;
*head=*head->link;
free(temp);
temp=0;
}
}

//add at par pos

void add_pos(struct st *head,int pos)
{
struct st *new,*temp=head;
new=malloc(sizeof(struct st));
new->link=0;

pos--;
while(pos!=1)
{
temp=temp->link;
pos--;
}
new->link=temp->link;
temp->link=new;
}

