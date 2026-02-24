#include <iostream>
#include <string>
using namespace std;

class Hdfc {
    int accno;
    string name;
    float bal;
    int pinNo;
public:
    Hdfc();
    int verifyPin();
    void deposit(float);
    bool withdrawal(float);
    void balEnq();
    void print();
    friend bool Shopping :: payment(Hdfc &);
};

