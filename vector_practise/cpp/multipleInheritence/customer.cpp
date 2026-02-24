#include "customer.h"

int customer :: customerId = 0;

void customer :: inputCustomerDetails() {
	customerId++;
	cout << "Enter the customer name :" << endl;
	cin >> name;
	cout << "Enter the delivery address : " << endl;
	cin >> address;
}

void customer :: displayCustomerDetails() {
	cout << "Customer id is : " << customerId << endl;
	cout << "Customer name is : " << name << endl;
	cout << "Customer address is : " << address << endl;
}

