#include <stdio.h>
void main() {
	int num1,num2;
	printf("Enter the first number: ");
	scanf("%d",&num1);
	printf("Enter the second number: ");
	scanf("%d",&num2);
	(num1 == num2) ? printf("Equal numbers") : (num1 > num2) ? printf("%d is greater number",num1) : printf("%d is the greater number",num2);
}
