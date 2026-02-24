#include <iostream>

using namespace std;
int main() {
	wchar_t ch;
	setlocale(LC_CTYPE,"");
	for(ch = 230;ch <= 2400;ch++) {
		wcout<<ch<<" ";
	}
	cout << endl;
}
