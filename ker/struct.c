#include<stdio.h>
typedef struct passing_vals{
       	int a;
       	int b;
	char c;   
}pv;
int main(){
	pv s={45,55,+};
	char buff[100];
	FILE *fp=open("/dev/open_close","w+");
	snprintf(buff,sizeof(buff),"%d%d%c",s.a,s.b,s.c);
	fwrite(buff,sizeof(pv),1,fp);
	//fread();
	fclose(fp);

}
