/**
 * @brief: 验证子类(Derived)，继承父类(Base)的同时，复合(Component)类；
 *          探究 Derived 内部 Base 成分先构造，还是 Component 成分先构造；
 * @author: liangj.zhang
 * @date: 9/6/2026
 * 
 * @summary: Derived 类对象内部，Base 成分先于 Component 成分先构造;
 */

/*
test1 output:

=== test1 === begin()
Base ctor ++
Component ctor, value = -1
Component dtor, value = -1
Base dtor ++
=== test1 === end()
*/

#pragma once

#include "Base.hpp"
#include "Component.hpp"

namespace test1 {

class Derived : public Base {

private:
    Component comp;
public:
    Derived() {}
};

void test() {
    Derived d;
}

} // namespace test1;