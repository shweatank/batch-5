#include <stdio.h>
#include <string.h>
int anagram(const char *s1,const char *s2);
int main() {
        int size;
        printf("Enter the size: ");
        scanf("%d",&size);
        char s1[size],s2[size];
        printf("Enter the string-1: ");
        scanf("%s",s1);
        printf("Enter the string-2: ");
        scanf(" %s",s2);
	int res = anagram(s1,s2);
	(res) ? printf("The given strings are anagram\n") : printf("Not Anagram\n");
}

int anagram(const char *s1,const char *s2) {
	int len1 = strlen(s1),len2 = strlen(s2),count = 0;
	if(len1 != len2) return 0;
	for(int i = 0;i<len1;i++) {
		for(int j = 0;j<len2;j++) {
			if (s1[i] == s2[j]) count++;
		}
	}
	return count == len1;
}
