#include <iostream>
#include "load.h"


class Empty {
public:
	Empty() {
		std::cout << "default ctor is called. --- " << std::hex << (unsigned long)this  << std::endl;
	}

	Empty(Empty& rhs) {
		std::cout << "copy ctor is called. --- " << std::hex << (unsigned long)this << std::endl;
	}

	Empty& operator=(const Empty& rhs) {
		std::cout << "copy assignment is called. --- " << std::hex << (unsigned long)this << std::endl;
		return *this;
	}

	~Empty() {
		std::cout << "dctor is called. --- " << std::hex << (unsigned long)this << std::endl;
	}
};


int main(int argc, char* argv[]) {

	std::cout << "sizeof Empty class: " << sizeof(Empty) << std::endl;
	std::cout << ">>>>> Empty e1; <<<<<-s" << std::endl;
	Empty e1;
	std::cout << ">>>>> Empty e1; <<<<<-e" << std::endl;
	std::cout << "sizeof Empty class' object: " << sizeof(e1) << std::endl;
	std::cout << std::endl;

	std::cout << ">>>>> Empty e2(e1); <<<<<-s" << std::endl;
	Empty e2(e1);
	std::cout << ">>>>> Empty e2(e1); <<<<<-e" << std::endl;
	std::cout << std::endl;

	std::cout << ">>>>> Empty e3; e3 = e1; <<<<<-s" << std::endl;
	Empty e3;
	e3 = e1;
	std::cout << ">>>>> Empty e3; e3 = e1; <<<<<-e" << std::endl;
	std::cout << std::endl;
	
	std::cout << ">>>>> Empty e4 = e1; <<<<<-s" << std::endl;
	Empty e4 = e1;
	std::cout << ">>>>> Empty e4 = e1; <<<<<-e" << std::endl;
	std::cout << std::endl;

	std::cout << "----- before return 0 -----" << std::endl;

	return 0;
}
