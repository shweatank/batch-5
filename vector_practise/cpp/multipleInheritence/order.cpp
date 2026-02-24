#include "order.h"

using namespace std;

void order :: inputOrderDetails() {
	cout << "Enter the quantity of product : " << endl;
	cin >> quantity;
}

void order :: calculateTotalAmount() {
	totalAmount = price * quantity;
}

void order :: displayOrderDetails() {
	displayProductDetails();
	displayCustomerDetails();
	cout << "Total quantity of product is : " << quantity << endl;
	cout << "Total amount of product is : " << totalAmount << endl;
}

