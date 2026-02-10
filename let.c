#include<stdio.h>
#include<string.h>
int lengthOfLongestSubstring(char* ); 
int main()
{
	char s[100];
	scanf("%s",s);
	lengthOfLongestSubstring(s);
}
int lengthOfLongestSubstring(char* s) {
    char str[200];
    char *p;
    int k,l;
    int i=0,j;
    int clen=0,blen=-1,cnt;
    if(s[0]=='\0')
    return 0;
    while(s[i])
    {
	    clen=0;
	    if(i!=(strlen(s)-1))
	    p=strchr(s+i+1,s[i]);
	    if(p!=NULL)
	    {
		    j=p-s;
	    k=i;
	    for(;k<j;k++)
	    {
		    cnt=0;
		    for(l=k;l<j;l++)
		    {
			    if(s[k]==s[l])
				    cnt++;
		    }
		    if(cnt==1)
			    clen++;
	    }
	    if(clen>blen)
		    blen=clen;
	    }

	    i++;
    }
    return blen;
}


