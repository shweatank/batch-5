#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void *zigzag(char *p,int row){
	int i,len=strlen(p),j,k;
	int id=0;
	char **ptr=NULL;
	char *s=NULL;
	ptr=calloc(1,row*sizeof(char *));
	if(len==1&&row==1){
		s=malloc(len);
		strcpy(s,p);
		return s;
	}
	for(int i=0;i<row;i++){
		ptr[i]=calloc(1,len/2*sizeof(char));
	}
	
	for(j=0;p[id];j++){
		for(i=0;i<row;i++){
			if(p[id]=='\0')
				break;
			ptr[i][j]=p[id++];
		}
		i--;
		for(k=0;k<row-2;k++){
			j++;
			i--;
			if(p[id]=='\0')
				break;
			ptr[i][j]=p[id++];
		}
		if(p[id]=='\0')
			break;
	}

	/*for(int i=0;i<row;i++){
		for(int j=0;j<len/2;j++){
			printf("%c\n",ptr[i][j]);
		}
	}*/
	s=malloc(len+1);
	k=0;
	for(int i=0;i<row;i++){
		for(int j=0;j<len/2;j++){
			if(ptr[i][j])
			s[k++]=ptr[i][j];
		}
	}
	s[k]='\0';
	for(int i=0;i<row;i++){
		free(ptr[i]);
	}
	free(ptr);
	return s;


}

int main(){
	char ch[]="PAYPALISHIRING";
	char ch1[]="A";
	char *p=zigzag(ch1,1);
	printf("%s\n",p);
	free(p);
	//printf("%ld\n",strlen(ch));
}

