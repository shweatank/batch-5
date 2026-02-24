#include <stdio.h>
void main() {
	unsigned char ch = 50;
	printf("ch = %c...%d...%x...%o\n",ch,ch,ch,ch);
	ch = ch*2;
	printf("ch = %c...%d...%x...%o\n",ch,ch,ch,ch);
	ch = ch - 32;
	printf("ch = %c...%d...%x...%o\n",ch,ch,ch,ch);
	printf("Enter the character: ");
	ch = getchar();
	printf("ch = %c...%d...%x...%o\n",ch,ch,ch,ch);
}

