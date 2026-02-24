#include <iostream>
#include <string>
using namespace std;
//class Doctor;
class Patient {
        string name;
        string healthStatus;
        int age;
	long phoneno;
        public:
        Patient();
        void print();
       // void health_status();
	friend void Doctor :: updateStatus(Patient &);
};

