#include <iostream>
#include <string>
using namespace std;
class student {
	int rollno;
	string name;
	float subjectMarks[6];
	float totalMarks;
	float percentage;
	float grade;
	public:
	void input() {
		cout << "Enter the rollno: " << endl;
		cin >> rollno;
		cout << "Enter the name: " << endl;
		cin >> name;
		cout << "Enter the 6 subjects marks: " << endl;
		for(int i = 0; i < 6;i++) {
			cin >> subjectMarks[i];
		}
	}
	void totalM() {
		totalMarks = 0;
		for(int i = 0;i<6;i++) {
			totalMarks += subjectMarks[i];
		}
		cout << totalMarks << endl;
	}
};

int main() {
	student stuobj;
	stuobj.input();
	stuobj.totalM();
}

