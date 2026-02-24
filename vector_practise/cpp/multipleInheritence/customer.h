#include <iostream>
#include <string>

using namespace std;

class customer {
	static int customerId;
	string name;
	string address;
	public:
		void inputCustomerDetails();
		void displayCustomerDetails();
};

