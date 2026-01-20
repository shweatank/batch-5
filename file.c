#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
int main()
{
  /*
  FILE *fp=fopen("f1","rb");
  if(!fp)
  {
     perror("fopen");
     return 0;
  }
  char s[20];
  int *p;
  fread(s,20,sizeof(s),fp);
//  fread(p,20,4,fd); 
//  write(fd,s,strlen(s)+1);
//  read(fd,s,sizeof(s));
//  printf("%s\n",s);
  fclose(fp);    
  */
   int fd=open("f1",O_RDWR);
   if(fd<0)
  {
    perror("open");
    return 0;
  }
  int a=5;
  write(fd,&a,sizeof(a));
  read(fd,&a,sizeof(a));
  printf("%d\n",a);          
}  
