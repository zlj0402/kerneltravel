/**
 * @brief: test1, 能够证明：
 *      + 类成员在没有在构造函数中构造时，至少是比构造函数的构造体，要早的；
 *      + 析构时，外部类（范围更大的类）先析构，内部的后析构；
 * @author: liangj.zhang
 * @date: 2/6/2025
 */

/*
test1 output:
=== test1 === begin()
Component ctor, value = -1
Container ctor
Container dtor-
Component dtor, value = -1
=== test1 === end()
*/

#include "Component.hpp"

namespace test1 {

class Container {
private:
    int ca;
    Component cc;
public:
    Container(int a = -2) : ca(a) {
        std::cout << "Container ctor" << std::endl;
    }

    ~Container() {
        std::cout << "Container dtor" << "-" << std::endl;
    }
};

void test() {
    Container container;
}

}