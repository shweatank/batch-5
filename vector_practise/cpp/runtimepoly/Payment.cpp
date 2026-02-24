#include <iostream>
#include <string>

using namespace std;

class Payment {
	protected:
		float amount;
		string paymentDate;
	public:
		Payment() {
			cout << "Enter the amount to be paid: ";
			cin >> amount;
			cout << "Enter the payment date: ";
			cin >> paymentDate;
		}
		virtual float processPayment() = 0;
		virtual void display() = 0;
};

