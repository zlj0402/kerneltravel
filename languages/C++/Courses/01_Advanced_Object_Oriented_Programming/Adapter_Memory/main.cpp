/*
output:
=== test1 === begin()
Component ctor, value = -1
Container ctor
Container dtor-
Component dtor, value = -1
=== test1 === end()
=== test2 === begin()
Component ctor, value = -1
Component ctor, value = 33
Container ctor
Container dtor+
Component dtor, value = 33
Component dtor, value = -1
=== test2 === end()
*/

/**
 * @brief: 感悟，先在 test1.hpp, test2.hpp 当中写了；
 * 
 * @summary: 变量获得定义的顺序(在内存中获得自己的空间)，就是他们声明的顺序；
 *          有在构造函数中的初始化列表中构造，定义时以构造的方式构造，函数体中过程是所有变量获得定义之后进行的；
 */

#include "./test1.hpp"
#include "./test2.hpp"

int main() {
    std::cout << "=== test1 === begin()" << std::endl;
    test1::test();
    std::cout << "=== test1 === end()" << std::endl;

    
    std::cout << "=== test2 === begin()" << std::endl;
    test2::test();
    std::cout << "=== test2 === end()" << std::endl;
}