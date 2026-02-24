#include <cstring>
Shopping::Shopping() {
    products = NULL;
    totalAmount = 0;
    totalItems = 0;
    productCount = 0;
}

void Shopping::addToCart() {
    products = (Product *)realloc(products,sizeof(Product) * (productCount+1));
    cout << "Enter the product name: ";
    cin >> products[productCount].name;
    cout << "Enter the product price: ";
    cin >> products[productCount].price;
    cout << "Enter the product quantity: ";
    cin >> products[productCount].quantity;

    totalAmount += (products[productCount].price * products[productCount].quantity);
    totalItems += products[productCount].quantity;
    productCount++;

    cout << "Product added successfully!\n";
}

void Shopping::viewCart() {
    if (productCount == 0) {
        cout << "Cart is empty!\n";
        return;
    }

    cout << "S.No   ProductName   Quantity   TotalAmt\n";
    for (int i = 0; i < productCount; i++) {
        cout << i + 1 << "      " << products[i].name
             << "            " << products[i].quantity
             << "           " << (products[i].price * products[i].quantity)
             << endl;
    }
    cout << endl << "Total Bill: " << totalAmount << endl;
    cout << "Total Items: " << totalItems << endl;
    cout << "Total Products: " << productCount << endl;
}

void Shopping::deleteProduct() {
    if (productCount == 0) {
        cout << "Cart is empty!\n";
        return;
    }

    int sno;
    cout << "Enter the S.No of product: ";
    cin >> sno;

    if (sno <= 0 || sno > productCount) {
        cout << "Invalid product number!\n";
        return;
    }
    memmove(products+(sno-1),products+sno,sizeof(Product)*(productCount-sno));
    totalAmount -= (products[sno - 1].price * products[sno - 1].quantity);
    totalItems -= products[sno - 1].quantity;
    productCount--;

    cout << "Product deleted successfully!\n";
}

int Shopping :: getProductCount() {
	return productCount;
}

bool Shopping::payment(Hdfc &hdfc) {
    if (hdfc.verifyPin()) {
        if (totalAmount <= hdfc.bal) {
            hdfc.bal -= totalAmount;
            cout << "Payment successful!" << endl;
            return true;
        } else {
            cout << "Insufficient balance. Payment failed.\n";
            return false;
        }
    } else {
        cout << "Invalid PIN. Payment cancelled.\n";
        return false;
    }
}

void Shopping::resetCart() {
    totalAmount = 0;
    totalItems = 0;
    productCount = 0;
}

