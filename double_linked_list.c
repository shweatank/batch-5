#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct node
{
  int data;
  char s[20];
  float f;
  struct node*next; 
  struct node*prev;
}DLL; 
void add_begin(DLL**head);
void print_data(DLL*head);
void add_end(DLL**head);
int count_nodes(DLL*head);
void add_middle(DLL**head);
void delete_begin(DLL**head);
void delete_end(DLL**head);
void delete_middle(DLL*head);
void delete_based_data(DLL**head);
void reverse_links(DLL**head);
void duplicate_data_delete(DLL*head);
void duplicate_data_delete(DLL*head);
void using_goto(DLL*head);
void for_loop(DLL*head);
void recursion_print(DLL*head);
void reverse_recursion_print(DLL*head);
void delete_node_data(DLL**head);
int main()
{
   DLL*head=NULL; 
   int op;
  while(1)
 {
   printf("1)add at begin\n");
   printf("2)print data\n");
   printf("3)add at end\n");
   printf("4)count nodes\n");
   printf("5)add at middle\n");
   printf("6)delete at begin\n"); 
   printf("7)delete at end\n");
   printf("8)delete at middle\n");
   printf("9)delete_based_data\n");
   printf("10)reverse_links\n");
   printf("11)duplicate data delete\n");
   printf("12)using goto\n");
   printf("13)for_loop\n");
   printf("14)recursion\n");
   printf("15)reverse recursion\n");
   printf("16)delete node data\n");
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
            printf("count=%d\n",k);
            break;
     case 5:add_middle(&head);
            break;
     case 6:delete_begin(&head);
            break;
     case 7:delete_end(&head);
            break;
     case 8:delete_middle(head);
            break;
     case 9:delete_based_data(&head);
            break;
     case 10:reverse_links(&head);
            break;
     case 11:duplicate_data_delete(head);
            break;
     case 12:using_goto(head);
             break;
     case 13:for_loop(head);
             break;
     case 14:recursion_print(head);
             break;
     case 15:reverse_recursion_print(head);
             break;
     case 16:delete_node_data(&head);
             break;
     default:printf("UNKNOW OPTION \n");
            break;
  }
 }
}
void add_begin(DLL**head)
{
  DLL*temp,*newnode;
  newnode=(DLL*)calloc(1,sizeof(DLL));
  printf("enter the data\n");
  scanf("%d %s %f",&newnode->data,newnode->s,&newnode->f);
  if(*head==NULL)
  {
	  *head=newnode;
  }
  else
  {
	  temp=newnode;
	  newnode->next=*head;
	  (*head)->prev=newnode;
	  *head=newnode;
  }
}
void print_data(DLL*head)
{
	DLL*temp=head;
	if(head==NULL)
	{
		printf("NO RECORDS\n");
		return ;
	}
	else
	{
		while(temp!=NULL)
		{
			printf("%d %s %f->",temp->data,temp->s,temp->f);
			temp=temp->next;
		}
	}
}         
void add_end(DLL**head)
{
	DLL*temp,*newnode;
	newnode=(DLL*)calloc(1,sizeof(DLL));
	printf("enter the data\n");
	scanf("%d %s %f",&newnode->data,newnode->s,&newnode->f);
	if(*head==NULL)
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
		newnode->prev=temp;
		temp->next=newnode;
	}
}
int count_nodes(DLL*head)
{
  DLL*temp=head;
  int c=0;
  if(head==NULL)
  {
    printf("NO ROCORDS\n");
    return 0;
  }
 else
 {
       while(temp!=NULL)
      {
         c++;
        temp=temp->next;
      }
 }
 return c;
} 
void add_middle(DLL**head)
{
  
	DLL*temp,*newnode;
	newnode=(DLL*)calloc(1,sizeof(DLL));
	printf("enter the data\n");
	scanf("%d %s %f",&newnode->data,newnode->s,&newnode->f);
	if(*head==NULL)
	{
      		*head=temp=newnode;
	}
	else if((*head)->next==0)
	{
           newnode->prev=*head;

           (*head)->next=newnode;
        }
        else
       {   
          int i=0;
          int c=count_nodes(*head);
          temp=*head;
          while(i<c/2)
         {
           temp=temp->next;
           i++;
         }
         newnode->next=temp;
         newnode->prev=temp->prev;
         temp->prev->next=newnode;
         temp->prev=newnode;
       }
}
void delete_begin(DLL**head)
{
  DLL*temp=*head;
  if(*head==NULL)
  {
    printf("NO RECORDS\n");
    return ;
  }
  else if((*head)->next==NULL)
 {
    *head=0;
    free(temp);
 } 
  else
 {
   *head=(*head)->next;
    (*head)->prev=0;
    free(temp);
 }
} 
void delete_end(DLL**head)
{
  DLL*temp=*head;
  if((*head)->next==NULL)
 {
   *head=0;
   free(temp);
 }
 else
{
   while(temp->next!=NULL)
  {
     temp=temp->next;
  }
  temp->prev->next=temp->next;
  temp->prev=0;
  free(temp);  
}
}                              
void delete_middle(DLL*head)
{
   DLL*temp=head;
  if(head==NULL)
  {
    printf("NO RECORDS \n");
    return ;
  }
 else
 {
   int i=0;
   int c=count_nodes(head);
    temp=head;
   while(i<c/2)
  {
    temp=temp->next;
    i++;
  }
  temp->prev->next=temp->next;
  temp->next->prev=temp->prev;
 }
}
void delete_based_data(DLL**head)
{
	DLL*temp=*head;
       int data;
       printf("enter the data\n");
       scanf("%d",&data);
	while(temp!=NULL)
	{
		if(temp->data==data)
		{
			if((*head)==temp)
			{
				if((*head)->next==NULL)
				{
					*head=0;
					free(temp);
				}
				else
				{
					*head=(*head)->next;
                                        (*head)->prev=0;
					free(temp);
				}
			}
			else if(temp->next==NULL)
			{
				temp->prev->next=temp->next;
				free(temp);

			}
			else
			{
				temp->prev->next=temp->next;
				temp->next->prev=temp->prev;
				free(temp);
			}
			return;
		}      
		temp=temp->next;
	} 
	printf("INNCORRECT DATA\n");
}
void reverse_links(DLL**head)
{
	DLL*nextnode=*head,*curr=*head;
	DLL*temp=*head,*prev=0;
       while(temp->next!=NULL)
       {
         temp=temp->next;
       }       
	while(nextnode!=NULL)
	{
		nextnode=curr->next;
		curr->next=curr->prev;
              //  prev=curr;
		curr->prev=nextnode;
		curr=nextnode;
	}
	*head=temp;

}                                                                                                                                                 
void duplicate_data_delete(DLL*head)
{
   DLL*p1,*p2,*temp;
   p1=head;
   int data;
   while(p1!=NULL)
  {
    p2=p1->next;
   temp=p1;
   while(p2!=NULL)
  {
    if(p1->data==p2->data)
   {
     if(p2->next==NULL)
    {
     temp=p2;
     p2->prev->next=p2->next;
     p2=p2->next;
     free(temp);
    }
    else
   {
      temp=p2;
      p2->prev->next=p2->next;
      p2->next->prev=p2->prev;
      p2=p2->next;
      free(temp);
   }
  }
   else
  {
     p2=p2->next;
  }
 }
 p1=p1->next;
}
}
void using_goto(DLL*head)
{
  DLL*temp=head;
   if(head==NULL)
  {
    printf("NO RECORDS FOUND\n");
    return;
  }   
 l:
 printf("%d %s %f->",temp->data,temp->s,temp->f);
 temp=temp->next;
 if(temp!=NULL)
 {
   
    goto l;
 }         
}           
void for_loop(DLL*head)
{
   int i,j=0;
   int c=count_nodes(head);
   DLL*temp=head;
   l:  
  for(i=0;i<c;i++)
 {
    temp=head; 
    printf("%d %s %f->",temp->data,temp->s,temp->f);
    temp=head;
   }
 }
}
void recursion_print(DLL*head)
{
  DLL*temp=head;
  if(temp==NULL)
  return ;
  else
 {
   printf("%d %s %f->",temp->data,temp->s,temp->f);
   recursion_print(temp->next);
 }
}
void reverse_recursion_print(DLL*head)
{
   DLL*temp=head;
  if(temp==NULL)
  return ;
  else
 {
   reverse_recursion_print(temp->next);
   printf("%d %s %f->",temp->data,temp->s,temp->f);
 }
}
void delete_node_data(DLL**head)
{
  DLL*temp=*head;
  int data;
  printf("enter the data\n");
  scanf("%d",&data); 
  while(temp!=NULL)
 {  
  if(temp->data==data)
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
     *head=(*head)->next;
      (*head)->prev=0;
      free(temp);
   }  
  }
  else if(temp->next==NULL)
  {
	  temp->prev->next=temp->next;
	  free(temp);
  }
  else
  {
	  temp->prev->next=temp->next;
	  temp->next->prev=temp->prev;
	  free(temp);
  }
 }
 temp=temp->next;
}
}                        
