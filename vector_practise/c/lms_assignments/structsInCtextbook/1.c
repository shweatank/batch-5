#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio_ext.h>
#define MAX 100
int count;
typedef struct {
	long int accNo;
	char name[100];
	float balance;
}BANK;

void addCustomer(BANK *bank) {
	if (count == MAX) {
		printf("Bank is full and unable to add new customers\n");
		return;
	}
	printf("Enter the account number: ");
	scanf("%ld",&bank[count].accNo);
	printf("Enter the customer name: ");
	scanf("%s",bank[count].name);
	printf("Enter the initial balance: ");
	scanf("%f",&bank[count].balance);
	count++;
}

void customersBelow100Balance(BANK *bank) {
	for(int i = 0;i<count;i++) {
		if(bank[i].balance < 100.0) {
			printf("%ld	%s\n",bank[i].accNo,bank[i].name);
		}
	}
}

void printCustomers(BANK *bank) {
	for(int i = 0;i<count;i++) {
		printf("%ld	%s	%f\n",bank[i].accNo,bank[i].name,bank[i].balance);
	}
}

void depositOrWithdraw(BANK *bank) {
	for(int i = 0;i<count;i++) {
		long int acno;
		printf("Account Number: ");
		scanf("%ld",&acno);
		if(bank[i].accNo == acno) {
			char c;
			float amount;
			printf("Enter the details\n");
			printf("Enter the amount: ");
			scanf("%f",&amount);
			printf("1.deposit\n0.withdrawl\n");
			__fpurge(stdin);
			scanf("%c",&c);
			switch(c) {
				case '0':
					if(bank[i].balance < amount) {
						printf("The balance is insufficient for the specified operation\n");
						return;
					}
					int res = bank[i].balance - amount;
					bank[i].balance = res;
					printf("Sucessfully Withdrawn\n");
					break;
				case '1':
					res = bank[i].balance + amount;
					bank[i].balance = res;
					printf("Sucessfully Deposited\n");
					break;
				default:
					printf("Invalid choice\n");
			}
			return;
		}
	}
	printf("Account not found\n");
	return;
}
					

int main() {
	BANK bank[MAX];
	char ch;
	while(1) {
		printf("a.add_customer p.print_all_customers l.print_customers_whose_balance_is_below_100 d.deposit_or_withdrawl e.exit\n");
		__fpurge(stdin);
		printf("Enter the choice: ");
		scanf("%c",&ch);
		switch(ch) {
			case 'a':
				addCustomer(bank);
				break;
			case 'l':
				customersBelow100Balance(bank);
				break;
			case 'p':
				printCustomers(bank);
				break;
			case 'd':
				depositOrWithdraw(bank);
				break;
			case'e':
				exit(0);
		}
	}
}
