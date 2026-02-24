#include <iostream>
#include <string>
using namespace std;
class Hdfc;

typedef struct {
    char name[20];
    float price;
    int quantity;
} Product;

class Shopping {
    Product *products;
    float totalAmount;
    int totalItems;
    int productCount;
public:
    Shopping();
    void addToCart();
    void viewCart();
    void deleteProduct();
    int getProductCount();
    void resetCart();
    bool payment(Hdfc &);
};

