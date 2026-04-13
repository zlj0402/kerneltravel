/**
 * @brief: const A a; 调用方法只能调用 const 成员函数；
 */

#include <iostream>
using std::cout;
using std::endl;

class A {
public:
    A(int a = 0) {
        this->a = a;
    }

    int getA() const {
        return a;
    }
    
    int a;
};

int main() {
    const A a;
    std::cout << a.getA() << std::endl;
}