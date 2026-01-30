#include<stdio.h>
#include<stdlib.h>

struct st{
	char name[10];
	int age;
	char loc[10];
	char prof[10];
}s;

int main(){
	FILE *fp=fopen("file1","r"),*fp1=NULL;
	int c;
	int cnt=0,d=0;
	if(fp==NULL){
		printf("error\n");
		exit(0);
	}
	while(1){
		c=fscanf(fp,"%s %d %s %s",s.name,&s.age,s.loc,s.prof);
		d=0;
		if(c==4){
		printf("%s %d %s %s\n",s.name,s.age,s.loc,s.prof);
		cnt++;
		//c=fscanf(fp,"%s %d %s %s",s.name,&s.age,s.loc,s.prof);
	//	printf("c=%d cnt=%d\n",c,cnt);
		continue;
		//fclose(fp);
		}
		while(c!=4){
			/*fp1=fopen("file1","r");
			do{			
				d++;
				c=fscanf(fp1,"%s %d %s %s",s.name,&s.age,s.loc,s.prof);
			//	if(d==cnt)
			//		break;
			}while(d<=cnt);
			//printf("c=%d d=%d\n",c,d);
			
			//c=fscanf(fp1,"%s %d %s %s",s.name,&s.age,s.loc,s.prof);
			if(c==4){
				fp=fp1;
				fp1=NULL;
		//		fclose(fp1);
				break;
			}*/
			c=fscanf(fp,"%s %d %s %s",s.name,&s.age,s.loc,s.prof);
			printf("c=%d cnt=%d\n",c,cnt);
		}
	/*	if(fp1!=NULL){
			d=0;
			fp=fp1;
			fclose(fp1);
			fp1=NULL;
		}*/
		printf("%s %d %s %s\n",s.name,s.age,s.loc,s.prof);
		
	}
	fclose(fp);

}

