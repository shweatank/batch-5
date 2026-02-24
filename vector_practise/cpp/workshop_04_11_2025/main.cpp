#include "doctor_class.h"
#include "patient_class.h"
#include "patient_class.cpp"
#include "doctor_class.cpp"

int main() {
	Doctor dct1;
	Patient pt1;
	dct1.print();
	pt1.print();
	dct1.updateStatus(pt1);
	pt1.print();
}
