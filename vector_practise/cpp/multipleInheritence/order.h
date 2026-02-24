#include <iostream>

class order : public product,public customer {
	int quantity;
	float totalAmount;
	public :
		void inputOrderDetails();
		void calculateTotalAmount();
		void displayOrderDetails();
};

