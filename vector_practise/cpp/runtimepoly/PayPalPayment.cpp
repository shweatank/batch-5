#include <iostream>
#include <string>

using namespace std;

class PayPalPayment: virtual public Payment {
        string email;
	string existingPassword;
	string enteredPassword;
        bool isVerified;
	void verifyPassword() {
		isVerified = (existingPassword == enteredPassword);
	}
	float processPayment() {
		verifyPassword();
		if(isVerified) {
	        	 return amount+5;
		}
		else {
			cout << "Wrong Password! Please try again" << endl;
			return 0.0;
		}
	}
        public:
                PayPalPayment() {
			existingPassword = "1234";
                        cout << "Enter the email : ";
                        cin >> email;
                        cout << "Enter the password : ";
                        cin >> enteredPassword;
                }
                void display() {
			cout << email << endl;
			cout << isVerified << endl;
			cout << processPayment() << endl;
                }
		~PayPalPayment() {
			cout << "pay pal payment destructor" << endl;
		}

};
