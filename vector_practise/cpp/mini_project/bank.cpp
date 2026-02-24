Hdfc::Hdfc() {
    srand(time(NULL));
    accno = 100000 + rand() % 500;
    /*
    cout << "Enter the name: ";
    cin >> name;
    cout << "Enter the initial balance: ";
    cin >> bal;
    cout << "Set your PIN: ";
    cin >> pinNo;
    */
    name = "Ajay";
    bal = 10000;
    pinNo = 1234;
}

int Hdfc::verifyPin() {
    int temp;
    cout << "Enter PIN for verification: ";
    cin >> temp;
    return (temp == pinNo);
}

void Hdfc::deposit(float amt) {
    if (verifyPin()) {
        bal += amt;
        cout << "Amount deposited successfully!\n";
    } else {
        cout << "Invalid PIN. Deposit cancelled.\n";
    }
}

bool Hdfc::withdrawal(float amt) {
    if (verifyPin()) {
        if (amt <= bal) {
            bal -= amt;
            cout << "Withdrawal successful!\n";
            return true;
        } else {
            cout << "Insufficient balance.\n";
            return false;
        }
    } else {
        cout << "Invalid PIN. Withdrawal cancelled.\n";
        return false;
    }
}

void Hdfc::balEnq() {
    if (verifyPin())
        cout << "Current Balance: " << bal << endl;
    else
        cout << "Invalid PIN.\n";
}

void Hdfc::print() {
    cout << "      Account Information     \n";
    cout << "Account No: " << accno << endl;
    cout << "Name: " << name << endl;
    cout << "Balance: " << bal << endl;
}

