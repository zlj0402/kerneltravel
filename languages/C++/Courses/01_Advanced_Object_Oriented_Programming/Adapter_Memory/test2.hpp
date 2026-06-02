/**
 * @brief: test2, 能证明：
 *      + 构造时，结合 test1，>>> 构造顺序只跟 类类型(Component)变量 在类当中(Container)声明的顺序有关；<<<
 *          + 即使是初始化列中的变量 cc，也比没有在构造函数定义的 cd 要迟一些构造；
 *      + 构造时，顺序是 由内而外的；（声明在前的最先构造，int ca -> cd -> cc）
 *      + 析构时，顺序是 由外而内的；（Container 是最先析构， cc -> cd -> int ca）
 *      + 析构顺序，跟构造顺序完全相反；
 */

/*
test2 output:
=== test2 === begin()
Component ctor, value = -1
Component ctor, value = 33
Container ctor
Container dtor+
Component dtor, value = 33
Component dtor, value = -1
=== test2 === end()
*/
#include "Component.hpp"

namespace test2 {

class Container {
private:
    int ca;
    Component cd;
    Component cc;
public:
    Container(int a = -2, int c = 33) : ca(a), cc(33) {
        std::cout << "Container ctor" << std::endl;
    }

    ~Container() {
        std::cout << "Container dtor" << "+" << std::endl;
    }
};

void test() {
    Container container;
}

}