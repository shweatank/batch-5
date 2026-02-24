#include <iostream>
#include <string>

using namespace std;

class CreditCardPayment: public Payment {
	string cardNumber;
	string cardHolderName;
	int cvv;
	public:
		CreditCardPayment() {
			cout << "Enter the credit card number : ";
			cin >> cardNumber;
			cout << "Enter the card holder name : ";
			cin >> cardHolderName;
			cout << "Enter the cvv: ";
			cin >> cvv;
		}
		float processPayment() {
			return (amount+amount*0.02);
		}
		void display() {
			cout << "**********" << cardNumber.substr(8,4) << endl;
			cout << cardHolderName << endl;
			cout << amount*0.02 << endl;
			cout << processPayment() << endl;
		}
		~CreditCardPayment() {
			cout << "credit card payment destructor" << endl;
		}
};


