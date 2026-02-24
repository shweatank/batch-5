#include <iostream>
#include <string>
using namespace std;
class Patient;

class Doctor {
	string name;
	string specalization;
	string emailid;
	public:
	Doctor();
	void print();
	void updateStatus(Patient &);
};
