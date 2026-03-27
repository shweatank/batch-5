int main()
{

        SLL *headptr=0;
while(1)
{
        printf("1.add_begin 2.print_data 3.exit 4.add_end 5.add_middile 6.sorting_data 7.count_perfect 8.count_palindrome\n");

        int op,count;

        printf("enter the option :\n");
        scanf("%d",&op);

        switch(op)
        {
                case 1:add_begin(&headptr);
                       break;
                case 2:print_data(headptr);
                       break;
                case 3 :
                           exit(0);
                case 4: add_end(&headptr);
                           break;
                case 5: add_middile(&headptr);
                        break;
                case 6:sorting_node(headptr);
                       break;
                case 7:count=perfect_count(headptr);
                       printf("number of perfect rollno=%d\n",count);
                       break;
                case 8: count=count_palindrome(headptr);
                        printf("number of palindromes =%d\n",count);
                        break;
                case 9:delete_name_node(&headptr);
                       break;

       }
}
}

 /*void add_begin(SLL **ptr)
 {
     SLL *new;//creating the node

   new = malloc(sizeof(SLL));//allocating the memory

   printf("enter the rollno name and marks:\n");
   scanf("%d %s %f",&new->rollno,new->name,&new->marks);//scanning the data
if(*ptr==0)
{
         *ptr=new;
        new->next=0;
}
 else
{
     new->next=*ptr; // update the new node
   *ptr=new;
}
 }
 */
void add_begin(SLL **ptr)
{
        SLL *new;
        new=malloc(sizeof(SLL));
        printf("enter the rollno name and marks:\n");
        scanf("%d %s %f",&new->rollno,new->name,&new->marks);
        if(*ptr==0)
                *ptr=new;
        else
                new->next=*ptr;
                *ptr=new;
}

void print_data(SLL *ptr)
{
        while(ptr!=0)
        {
                printf("%d %s %f\n",ptr->rollno,ptr->name,ptr->marks);
                ptr=ptr->next;
        }
}

void add_end(SLL **ptr)
{
        SLL *new;
        new=malloc(sizeof(SLL));
        printf("enter the rollno,name,marks\n");
        scanf("%d %s %f",&new->rollno,new->name,&new->marks);

        new->next=0;
        if(*ptr==0)
        {
                *ptr=new;
        }
        else
        {
                 //last=*ptr;
                while((*ptr)->next)
                          *ptr=(*ptr)->next;

                (*ptr)->next=new;
        }
}

void add_middile(SLL **ptr)
{
         SLL *new;
         new=malloc(sizeof(SLL));
         printf("ente the rollno name and marks:\n");
         scanf("%d %s %f",&new->rollno,new->name,&new->marks);

         if((*ptr==0)||(new->rollno<(*ptr)->rollno))
         {

                  new->next=0;
                  *ptr=new;
         }
         else
         {
                 SLL *last=*ptr;

                 while((last->next!=0)&&(new->rollno>last->next->rollno))
                         last=last->next;

                   new->next=last->next;
                   last->next=new;
         }
}


void sorting_node(SLL *ptr)
{
        int i,j,tempr,c=node_count(ptr);
        float tempf;
        char s[20];
     SLL *temp;

       for(i=0;i<c;i++)
       {

               temp=ptr->next;
               for(j=i+1;j<c;j++)
               {
                       if(ptr->rollno>temp->rollno)
                       {
                            tempr=ptr->rollno;
                            ptr->rollno=temp->rollno;
                            temp->rollno=tempr;

                            strcpy(s,ptr->name);
                            strcpy(ptr->name,temp->name);
                            strcpy(temp->name,s);

                            tempf=ptr->marks;
                            ptr->marks=temp->marks;
                            temp->marks=tempf;
                       }
                      temp=temp->next;

               }
               ptr=ptr->next;
       }

}

int node_count(SLL *ptr)
{
        int c=0;

for(ptr,c=0;ptr;c++,ptr=ptr->next);

return c;
}

int perfect_count(SLL *ptr)
{
        if(ptr==0)
        {
                printf("no nodes are present:\n");
                return 0;
        }

        int num,sum=0,i,c=0;

       while(ptr)
       {

               num=ptr->rollno;
               for(i=1,sum=0;i<num;i++)
               {
                       if(num%i==0)
                               sum+=i;
               }
               if(num==sum)
                       c++;


               ptr=ptr->next;
       }

          return c;

    }


int count_palindrome(SLL *ptr)
{

         if(ptr==0)
         {
                 printf("no records are present:\n");
                 return 0;
         }
       int i,j,len,c=0;
       char s[20];

       while(ptr)
       {
              strcpy(s,ptr->name);

              len=strlen(s);

              for(i=0,j=len-1;i<j;i++,j--)
              {
                      if(s[i]!=s[j])
                              break;
              }
              if(s[i]==s[j])
                      c++;
               ptr=ptr->next;
       }

       return c;
}

void delete_name_node(SLL **ptr)
{
        if(*ptr==0)
        {
                printf("no records are present:\n");
                return ;
        }

        char s[20];
        SLL *pre=*ptr;
        printf("enter the name to delete:\n");
        scanf("%s",s);

        while(*ptr!=0)
        {
                if(!(strcmp(s,(*ptr)->name)))
                {
                        pre->next=(*ptr)->next;

                        printf("node is deleted:\n");

                        return ;
                }

                pre=(*ptr)->next;
                *ptr=(*ptr)->next;
        }
      printf("no name is present in the database:\n");
}
