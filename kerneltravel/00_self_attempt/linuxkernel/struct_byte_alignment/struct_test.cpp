#include <iostream>
using std::cout;
using std::endl;

struct A {
	bool a;
	int b;
	bool c;
};

struct __attribute__((packed)) B {
	bool a;
	int b;
	bool c;
};

struct C {
	int b;
	bool a;
	bool c;
};

int main() {
	
	cout << sizeof(A) << endl;
	cout << sizeof(B) << endl;
	cout << sizeof(C) << endl;
	return 0;
}
