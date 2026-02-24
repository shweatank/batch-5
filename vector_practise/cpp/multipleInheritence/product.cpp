#include "product.h"

int product :: productId = 0;

void product :: inputProductDetails() {
	productId++;
	cout << "Enter the product name: " << endl;
	cin >> productName;
	cout << "Enter the price of product: " << endl;
	cin >> price;
}

void product:: displayProductDetails() {
	cout << "product id is : " << productId << endl;
	cout << "Product name is : " << productName << endl;
	cout << "Product price is : " << price << endl;
}

