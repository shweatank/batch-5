#include <iostream>

using namespace std;

class Array {
	int choice;
	int size;
	int array[5];
	public:
	Array() {
		cout << "enter the choice : " << endl;
		cin >> choice;
	}
	void 
	void sortArrayNumbers(int *arr) {
		for(int i = 0;i<size;i++) {
			for(int j = 0;j<size-i-1;j++) {
				if(arr[j] > arr[j+1]) {
					int temp = arr[j];
					arr[j] = arr[j+1];
					arr[j+1] = temp;
				}
			}
		}
	}

	void sortArrayChars(int *arr) {
		for(int i = 0;i<size;i++) {
			for(int j = 0;j<size-i-1;j++) {
				if(arr[j] > arr[j+1]) {
					int temp = arr[j];
					arr[j] = arr[j+1];
					arr[j+1] = temp;
				}
			}
		}
	}

};

int main() {
	Array arr;
}
				

