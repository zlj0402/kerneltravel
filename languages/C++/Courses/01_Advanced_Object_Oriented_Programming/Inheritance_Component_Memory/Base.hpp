#pragma once

#include <iostream>

class Base {
protected:
    Base() {
        std::cout << "Base ctor ++" << std::endl;
    }

    virtual ~Base() {
        std::cout << "Base dtor ++" << std::endl;
    }
};