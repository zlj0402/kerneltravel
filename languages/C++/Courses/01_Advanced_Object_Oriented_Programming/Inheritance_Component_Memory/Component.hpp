#pragma once
#include <iostream>

class Component {
private:
    int cb;
public:
    Component(int b = -1) : cb(b) {
        std::cout << "Component ctor, value = " << cb <<  std::endl;
    }

    ~Component() {
        std::cout << "Component dtor, value = " << cb << std::endl;
    }
};