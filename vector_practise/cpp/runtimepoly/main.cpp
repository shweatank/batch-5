#include "Payment.cpp"
#include "CreditCardPayment.cpp"
#include "PayPalPayment.cpp"

int main() {
	Payment *payment;
	char choice;
	while(1) {
		cout << "c|C.CreditCardPayment  p|P.PayPalPayment  e|E.Exit" << endl;
		cout << "Enter the choice: ";
		cin >> choice;
		switch(choice) {
			case 'c':
			case 'C':
				payment = new CreditCardPayment;
				payment->display();
				delete payment;
				break;
			case 'p':
			case 'P':
				payment = new PayPalPayment;
				payment->display();
				delete payment;
				break;
			case 'e':
			case 'E':
				exit(0);
			default:
				break;
		}
	}
}

