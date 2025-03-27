#include <iostream>
#include "load.h"

template<typename T>
class NamedObject {
public:
	NamedObject(T* obj) {
		this->objectPointer = obj;
		std::cout << "ctor called. --- " << (unsigned long)this << std::endl;
	}

	~NamedObject() {
		std::cout << "dtor called. --- " << (unsigned long)this << std::endl;
		delete this->objectPointer;
	}

private:
	T* objectPointer;

public:
	T getVal() const {
		return *(this->objectPointer);
	}

	void setVal(T val) {
		*(this->objectPointer) = val;
	}
};

int main(int argc, char* argv[])
{
	int a = 10;
	NamedObject<int> no1(&a);
	std::cout << "*(no1.objectPointer): " << no1.getVal() << std::endl;
	std::cout << std::endl;
	
	NamedObject<int> no2 = no1;
	std::cout << "copy ctor: NamedObject<int> no2 = no1;" << std::endl;
	std::cout << "*(no2.objectPointer): " << no2.getVal() << std::endl;
	std::cout << std::endl;

	no2.setVal(20);
	std::cout << "*(no2.objectPointer): " << no2.getVal() << std::endl;
	std::cout << std::endl;

	std::cout << "How about a and *(no1.objectPointer)? " << std::endl;
	std::cout << "a: " << a << std::endl;
	std::cout << "*(no1.objectPointer): " << no1.getVal() << std::endl;
}
