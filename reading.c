#include<stdio.h>
#include<string.h>
typedef struct details{
	char name[40];
	char place[40];
	int age;
	char prof[40];
}dts;

int main(){
	FILE *f,*m;
	dts p;
	int l=0;
	while(1){
		char ch;
        	scanf("%c",&ch);
		getchar();
	if(ch=='r'){
		f=fopen("data.txt","r");
		fseek(f,l,SEEK_SET);
		//printf("%d\n",l);
		fscanf(f,"%s%s%d%s",p.name,p.place,&p.age,p.prof);
		printf("%s, %s, %d, %s\n",p.name,p.place,p.age,p.prof);
		l=ftell(f);
		fclose(f);
	}		
	/*if(ch=='w'){
		puts("enter name");
		fgets(p.name,sizeof(p.name),stdin);
		p.name[strcspn(p.name, "\n")] = '\0';
		puts("enter location");
		fgets(p.place,sizeof(p.place),stdin);
                p.place[strcspn(p.place, "\n")] = '\0';
		puts("enter age"); 
	    	scanf("%d",&p.age);
		getchar();
		puts("enter profession");
 		fgets(p.prof,sizeof(p.prof),stdin);
                p.prof[strcspn(p.prof, "\n")] = '\0';
		m=fopen("data.txt","a");
		fprintf(m,"%s %s %d %s\n",p.name,p.place,p.age,p.prof);		
		fclose(m);
	}
	else if(ch=='e'){
		break;*/
	}

}

