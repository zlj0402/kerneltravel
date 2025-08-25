"""
Author: liangj.zhang
Created: 2025-08-25

Demonstration of Python class comparison behavior with and without __eq__ method.

This script compares two classes:
- Class A: Does not override __eq__, so '==' uses object.__eq__, which compares object identity (same as 'is').
- Class A_: Overrides __eq__, so '==' compares the value of 'val' attribute instead of object identity.

Key points:
- 'is' always checks if two references point to the same object (same memory address).
- By default, '==' falls back to object.__eq__, which uses 'is'.
- When __eq__ is implemented, '==' follows the user-defined logic.

References:
https://docs.python.org/3/reference/datamodel.html#object.__eq__

Expected output:
    a == b:  False
    a is b:  False
    a_ == b_:  True
    a_ is b_:  False
"""

from __future__ import annotations

class A:

    val = 0

    def __init__(self, val: int) -> None:
        self.val = val

class A_:

    val = 0

    def __init__(self, val: int) -> None:
        self.val = val
    
    def __eq__(self, other: A) -> bool:
        return self.val == other.val
    

a = A(10)
b = A(10)

print ("a == b: ", a == b)
print ("a is b: ", a is b)

a_ = A_(100)
b_ = A_(100)

print("a_ == b_: ", a_ == b_)
print("a_ is b_: ", a_ is b_)

""" 
output:
a == b:  False
a is b:  False
a_ == b_:  True
a_ is b_:  False

对比主要，还是要看 == 的对比：
A 没重载 == 之前，== 调用 class A 的父类 object.__eq__()

官网的一段描述：
URL：https://docs.python.org/3/reference/datamodel.html#object.__eq__
| By default, object implements __eq__() by using is, 
| returning NotImplemented in the case of a false comparison: True if x is y else NotImplemented.

A_ 对 == 重载之后，== 调用 class A_ 自己的 __eq__()
所以，a_ == b_:  True
"""