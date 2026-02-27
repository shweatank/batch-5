#include<stdio.h>
#include<stdlib.h>
struct node
{
	int d;
	struct node *link;
};
void print(struct node *p)
{
	while(p){
		printf("%d ",p->d);
		p=p->link;}
	printf("\n");
}
struct node* merge(struct node *merged,struct node *p1,struct node *p2)
{
	struct node head;
	merged=&head;
	while(p1 && p2)
	{
		if(p1->d < p2->d)
		{
			merged->link=p1;
			p1=p1->link;
		}
		else
		{
			merged->link=p2; 
			p2=p2->link;
		}
		merged=merged->link;
	}
	merged=(p1!=NULL)?p1:p2;
	return head.link;

}
int main()
{
	struct node *head1=NULL,*head2=NULL,*newnode=NULL,*merged=NULL;
	for(int i=0;i<3;i++)
	{
		int v;
		scanf("%d",&v);
		newnode=calloc(1,sizeof(struct node));
		newnode->d=v;
		if(head1==NULL)head1=newnode;
		else{newnode->link=head1;
			head1=newnode;}
	}
	print(head1);
	for(int i=0;i<6;i++)
	{
		int v;
		scanf("%d",&v);
		newnode=calloc(1,sizeof(struct node));
		newnode->d=v;
		if(head2==NULL)head2=newnode;
		else{newnode->link=head2;
			head2=newnode;}
	}
	print(head2);
	merged=merge(merged,head1,head2);
	print(merged);
	return 0;
}
