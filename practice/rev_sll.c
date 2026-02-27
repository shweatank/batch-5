#include<stdio.h>
#include<stdlib.h>
typedef struct st
{
	int dat;
	struct st *link;
}st;
void print(st *p)
{
	while(p!=NULL)
	{
		printf("%d ",p->dat);
		p=p->link;
	}
}
st *insert_begin(st *p,int d)
{
	st *nn=calloc(1,sizeof(st));
	nn->dat=d;
	if(p==NULL)
		p=nn;
	else
	{
		nn->link=p;
		p=nn;
	}
	return p;
}
st * reverse_sll(st *p)
{
	st *pptr=NULL,*cptr=NULL,*nptr=p;
	while(nptr)
	{
		pptr=cptr;
		cptr=nptr;
		nptr=nptr->link;
		cptr->link=pptr;
	
	}
	p=cptr;
	return p;
}
void find_middle(st *p)
{
	st *slow=p,*fast=p,*t=NULL;
	while(fast && fast->link)
	{
		t=slow;
		slow=slow->link;
		fast=fast->link->link;
	}
	if(fast!=NULL)
	{
		printf("mid node:%d\n",slow->dat);
	}
	else if(fast==NULL)
	{
		printf("mid nodes:%d %d\n",t->dat,slow->dat);
	}
}
int main()
{
	st *head=NULL;
	head=insert_begin(head,10);
	head=insert_begin(head,9);
	head=insert_begin(head,8);
	head=insert_begin(head,7);
	head=insert_begin(head,6);
	head=insert_begin(head,5);
	print(head);
	head=reverse_sll(head);
	print(head);
	find_middle(head);
	return 0;
}
