#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct node
{
  int data;
  char s[20];
  float f;
  struct node*next; 
}SLL; 
void add_begin(SLL**head);
void print_data(SLL*head);
void add_end(SLL**head);
int count_nodes(SLL*head);
void add_middle(SLL**head);
void delete_begin(SLL**head);
void delete_end(SLL*head);
void delete_middle(SLL*head);
void delete_duplicate(SLL*head);
void reverse_links(SLL**head);
void sort_data(SLL*head);
void serach_data(SLL*head);
void print_goto(SLL*head);
void recursion_print(SLL*head);
void reverse_using_recursion(SLL*head);
void delete_based_data(SLL**head);
void 
int main()
{
SLL*head=NULL;
  int op;
  while(1)
 {
   printf("1)add at begin\n");
   printf("2)print data\n"); 
   printf("3)add at end\n");
   printf("4)count the nodes\n");
   printf("5)add at middle\n"); 
   printf("6)delete at begin\n");
   printf("7)delete at end\n");
   printf("8)delete at middle\n");
   printf("9)delete_duplicate\n");
   printf("10)reverse links\n");
   printf("11)sort data\n"); 
   printf("12)search data\n"); 
   printf("13)print goto\n");
   printf("14)recursion print\n");
   printf("15)reverse recursion print\n");
   printf("16)delete based on data\n");    
   printf("enter the op\n");
   scanf("%d",&op);
   switch(op)
  {
     case 1:add_begin(&head);
            break;
     case 2:print_data(head);
            break;
     case 3:add_end(&head);
            break;
     case 4:int k=count_nodes(head);
            printf("%d\n",k); 
            break; 
     case 5:add_middle(&head);
            break;
     case 6:delete_begin(&head);
            break;
     case 7:delete_end(head);
            break;
     case 8:delete_middle(head);
            break;
     case 9:delete_duplicate(head);
            break; 
     case 10:reverse_links(&head);
             break;
     case 11:sort_data(head);
             break;
     case 12:serach_data(head);
             break;
     case 13:print_goto(head);
             break;
     case 14:recursion_print(head);
             break;
     case 15:reverse_using_recursion(head);
             break;
     case 16:delete_based_data(&head);
             break;
     default:printf("UNKNOWN OPTION..\n");
             break;
  }
 }
}
void add_begin(SLL**head)
{
  SLL*newnode,*temp;
  newnode=(SLL*)malloc(sizeof(SLL));
  newnode->next=0;
  printf("enter the data..\n");
  scanf("%d %s %f",&newnode->data,newnode->s,&newnode->f);  
  if((*head)==NULL)
  {
    *head=newnode;
  }
 else
 {
    temp=newnode;
    newnode->next=*head;
    *head=newnode;
 }
}              
void print_data(SLL*head)
{
   SLL*temp; 
  if(head==NULL)
  {
	  printf("NO NODES PRESENT..\n");
	  return ;
  }
  else
  {
	  temp=head;
	  while(temp!=NULL)
	  {
		  printf("%d %s %f->",temp->data,temp->s,temp->f);
		  temp=temp->next;
	  }
	  printf("NULL\n"); 
  }
}
void add_end(SLL**head)
{                                           
	SLL*newnode,*temp;
	newnode=(SLL*)malloc(sizeof(SLL));
	newnode->next=0;
	printf("enter the data..\n");
	scanf("%d %s %f",&newnode->data,newnode->s,&newnode->f);  
	if((*head)==NULL)
	{
		*head=newnode;
	}
	else
	{
                temp=*head;
		while(temp->next!=NULL)
		{
			temp=temp->next;
		}
		temp->next=newnode;
	}
}
int count_nodes(SLL*head)
{
   SLL*temp;
    int c=0;
   if(head==NULL)
  {
     printf("no nodes present\n");
     return 0;
  }
  else
 {
   temp=head;
   while(temp!=NULL)
  {
     c++;
    temp=temp->next;
  }
 }
 return c;
}              
void add_middle(SLL**head)
{
	SLL*newnode,*temp,*prev;
	newnode=(SLL*)malloc(sizeof(SLL));
	newnode->next=0;
	printf("enter the data..\n");
	scanf("%d %s %f",&newnode->data,newnode->s,&newnode->f);  
	if((*head)==NULL)
	{
		*head=newnode;
	}
	else if((*head)->next==NULL)
	{
           (*head)->next=newnode;
        }
       else
       {
         temp=*head;
         int i=0;
         int c=count_nodes(*head);
        while(i<c/2)
        {
           prev=temp; 
           temp=temp->next;
           i++;
        }
        prev->next=newnode;
        newnode->next=temp;  
       }
}
void delete_begin(SLL**head)
{
  SLL*temp=*head;
  if(*head==NULL)
 {
   printf("NO RECORDS..\n");
   return ;
 }
 else
{
  *head=temp->next;
   free(temp);
}
}
void delete_end(SLL*head)
{
  SLL*temp,*prev;
  if(head==NULL)
 {
   printf("NO RECORDS..\n");
   return ;
 }
 else
 {
    temp=head;
    while(temp->next!=NULL)
   {
      prev=temp;
      temp=temp->next;
   }
   prev->next=temp->next;
 }
 free(temp);
}
void delete_middle(SLL*head)
{
	int d,i;
        SLL*prev,*temp;
	if(head==NULL)
	{
		printf("NO RECORDS..\n");
		return ;
	}
	else
	{
               temp=head; 
		i=0; 
		printf("enter the d\n");
		scanf("%d",&d);
		while(i<d)
		{
			prev=temp;
			temp=temp->next;
			i++;
		}
		prev->next=temp->next;
		free(temp);
	}
}         
void delete_duplicate(SLL*head)
{
  SLL*temp,*p1,*p2,*prev;
  p1=head; 
  while(p1!=NULL) 
 {
   p2=p1->next;
   prev=p1;
   while(p2!=NULL)
  {
     if(p1->data==p2->data)
    {
      temp=p2;
      prev->next=p2->next;
      p2=temp->next;
      free(temp);
    }
   else
   {
     prev=p2;
     p2=p2->next;
   }
  }
  p1=p1->next;
}
}
void reverse_links(SLL**head)
{
  SLL*nextnode,*curr,*prev;
  if((*head)==NULL)
  {
     printf("NO RECORDS FOUND..\n");
     return ;
  }
  else
 {
    curr=*head;
    prev=0; 
    while(nextnode!=NULL)
   {
      nextnode=curr->next;
      curr->next=prev;
      prev=curr;
      curr=nextnode;
   }
   *head=prev;
 }
}                            
void sort_data(SLL*head)
{
	SLL*p1,*p2,*temp;
	p1=head;
	char s[20];
	int t;
	float f;  
	while(p1!=NULL)
	{
		p2=p1->next;
		while(p2!=NULL)
		{
			if(p1->data > p2->data)
			{
				t=p1->data;
				p1->data=p2->data;
				p2->data=t;
				strcpy(s,p1->s);
				strcpy(p1->s,p2->s);
				strcpy(p2->s,s);
				f=p1->f;
				p1->f=p2->f;
				p2->f=f;
			}
			p2=p2->next;
		}
		p1=p1->next;
	}
}    
void serach_data(SLL*head)
{
  SLL*temp=head;
  int data,flag=0;
   char s[20];
  if(head==NULL)
  {
    printf("no nodes present\n");
    return ;
  }
  else
{
   printf("enter the char\n");
   scanf("%s",s); 
   while(temp!=NULL)
  {
    if(strcmp(temp->s,s)==0)
    {
       printf("%s\n",temp->s);
       flag=1;
       break;
    }
   temp=temp->next;
  }
  if(flag==1)
 {
   printf("RECORD FOUND\n");
   return ;
 }
 else
 printf("RECORD NOT FOUND\n");
 
}
}
void print_goto(SLL*head)
{
  SLL*temp=head;
  int i=0,j,k;
  int c=count_nodes(head);
  l:
     printf("%d %s %f->",temp->data,temp->s,temp->f);
     temp=temp->next;
   if(temp!=NULL)
  {
	  goto l;
  }
}           
void recursion_print(SLL*head)
{
	SLL*temp=head;
	if(temp->next==NULL)
	{
		printf("%d %s %f->",temp->data,temp->s,temp->f);
		return;
	}
	else
	{
		printf("%d %s %f->",temp->data,temp->s,temp->f);
		recursion_print(temp->next);
	}
}   
void reverse_using_recursion(SLL*head)
{
	SLL*temp=head;
	if(temp->next==NULL)
	{
		printf("%d %s %f->",temp->data,temp->s,temp->f);
		return ;
	}
	else
	{
		reverse_using_recursion(temp->next);
		printf("%d %s %f->",temp->data,temp->s,temp->f);
	}
} 
void delete_based_data(SLL**head)
{
	SLL*temp,*prev;
	int data; 
         char s[20];
	printf("enter the data\n");
	scanf("%s",s);  
	temp=*head;
	while(temp!=NULL)
	{
		if(strcmp(temp->s,s)==0)
		{
			if(temp==*head)
			{
				if((*head)->next==NULL)
				{
					*head=0;
					free(temp);
				}
				else
				{
					*head=temp->next;
					free(temp);
				}
			}
			else if(temp->next==NULL)
			{
				prev->next=temp->next;
				free(temp);
				
			}
			else
			{
				prev->next=temp->next;
				free(temp);
				
			}
			printf("DATA FOUND\n");
			return ; 
		}
		prev=temp;
		temp=temp->next;
	}
	printf("DATA INCORRECT\n");
} 
                            
