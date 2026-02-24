#include "shopping.h"
#include "bank.h"
#include "bank.cpp"
#include "shopping.cpp"

int main() {
    Hdfc hdfc;
    Shopping shopping;
    char choice;
    float amt;

    while (true) {
        cout << "        MENU       \n";
        cout << "A. Add to Cart\n";
        cout << "V. View Cart\n";
        cout << "D. Delete Product\n";
        cout << "C. Checkout\n";
        cout << "E. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 'A':
            shopping.addToCart();
            break;
        case 'V':
            shopping.viewCart();
            break;
        case 'D':
            shopping.deleteProduct();
            break;
        case 'C': 
	    shopping.viewCart();
            if (shopping.getProductCount() == 0) {
                cout << "Your cart is empty!\n";
                break;
            }            
            if (shopping.payment(hdfc)) {
            	shopping.resetCart();
            }
            break;
        case 'E':
            exit(0);
        default:
            cout << "Invalid choice!\n";
        }
    }
}

