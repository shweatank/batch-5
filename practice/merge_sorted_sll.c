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
struct node * merge(struct node *merged,struct node *p1,struct node *p2)
{
	struct node *new=NULL,*temp=NULL;
	int t;
	while(p1 && p2)
	{
		new=calloc(1,sizeof(struct node));
		if(p1->d < p2->d)
		{
		new->d=p1->d;
		p1=p1->link;	
		}
		else
		{
			new->d=p2->d;
			p2=p2->link;
		}
		if(merged==NULL)
		{
			merged=temp=new;
		}
		else
		{
			temp->link=new;
			temp=new;
		}
	}
	while(p2){
		new=calloc(1,sizeof(struct node));
				new->d=p2->d;
				temp->link=new;temp=new;
				p2=p2->link;}

				
	while(p1){
		new=calloc(1,sizeof(struct node));
				new->d=p1->d;
				temp->link=new;temp=new;
				p1=p1->link;}
		return merged;

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
