#include <iostream>
#include "load.h"


class Empty {

	void func() {}

	virtual void virFunc() {}

	virtual void virFunc2() {}

	virtual void virFunc3() {}

	public:
		Empty() {}
};


int main(int argc, char* argv[]) {

	std::cout << "sizeof Empty class: " << sizeof(Empty) << std::endl;
	Empty empty;
	std::cout << "sizeof Empty class' object: " << sizeof(empty) << std::endl;

	return 0;
}
