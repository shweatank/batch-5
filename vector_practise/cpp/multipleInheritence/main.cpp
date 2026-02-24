#include "product.cpp"
#include "customer.cpp"
#include "order.cpp"

int main() {
	order ord;
	ord.inputProductDetails();
	ord.inputCustomerDetails();
	ord.inputOrderDetails();
	ord.calculateTotalAmount();
	ord.displayOrderDetails();
}

