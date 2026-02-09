/*9> remove kth node from an end of an singly linked list*/ 


//1 2 3 4 5

#include<stdio.h>
#include<stdlib.h>
#include<stdio_ext.h>

typedef struct st{
	int n;
	struct st *link;
}sll;

int cnt;


sll *addLast(sll *head){
	sll *node=NULL;
	node=calloc(1,sizeof(sll));
	if(node==NULL){
		printf("node not created\n");
		return head;
	}

	printf("enter data:");
	scanf("%d",&node->n);
	cnt++;

	if(head==NULL){
		head=node;
		return head;
	}

	sll *t=head;
	while(t->link){
		t=t->link;
	}
	t->link=node;
	return head;
}

void print(sll *head){
	if(head==NULL){
		printf("LIST IS EMPTY\n");
		return;
	}
	while(head){
		printf("%d ",head->n);
		head=head->link;
	}
	printf("\n");
}

sll *remKthNode(sll *head){

	int p,c=1;
	sll *prev=NULL,*next=NULL;
	if(head==NULL){
		printf("list is empty\n");
		return head;
	}

	printf("enter position:");
	scanf("%d",&p);
	

	next=head;
	prev=head->link;
	if(head&&c==p){
		next=head;
		head=head->link;
		free(next);
		return head;
	}	

	while(next&&c!=p){
		c++;
		prev=next;
		next=next->link;
	}
	
	if(c==p&&next){
		prev->link=next->link;
		return head;
	}
	if(next==NULL){
		printf("invalid position\n");
		return head;
	}

}
sll *remKthNodeFromEnd(sll *head){

	int p,c=1;
	sll *prev=NULL,*next=NULL;
	if(head==NULL){
		printf("list is empty\n");
		return head;
	}

	printf("enter position:");
	scanf("%d",&p);
	
	p=(cnt-p)+1;
	next=head;
	prev=head->link;
	if(head&&c==p){
		next=head;
		head=head->link;
		free(next);
		cnt--;
		return head;
	}	

	while(next&&c!=p){
		c++;
		prev=next;
		next=next->link;
	}
	
	if(c==p&&next){
		prev->link=next->link;
		cnt--;
		return head;
	}
	if(next==NULL){
		printf("invalid position\n");
		return head;
	}

}



int main(){
	sll *head=NULL;
	char ch;
	while(1){
		printf("a-addLast p-print k-removeFromBegining i-removeKthNodeFromEnd e-exit\n");
		printf("enter choice:");
		__fpurge(stdin);
		scanf("%c",&ch);	
		switch(ch){
			case 'a':head=addLast(head);break;
			case 'p':print(head);break;
			case 'k':head=remKthNode(head);break;
			case 'i':head=remKthNodeFromEnd(head);break;
			case 'e':exit(0);
		}
	}
}






