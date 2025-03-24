/**
 * @brief: Try to find out what access level default member functions have in a class. -- 24/3/2025
 * 
 * @discovery:
 * 		第一次，我写的时候，没有加任何的权限修饰符; 
 * 				显示写了一个无参构造，和一个void func() {}
 * 		直接make，进行编译的时候，出现如下错误:
 *			$ make
 *			g++ -x c++ -std=c++11 -o Empty.o -c Empty.cpp
 *			Empty.cpp: In function ‘int main(int, char**)’:
 *			Empty.cpp:16:8: error: ‘Empty::Empty()’ is private within this context
 *			   16 |  Empty empty;
 *			      |        ^~~~~
 *			Empty.cpp:7:2: note: declared private here
 *				7 |  Empty() {}
 *				  |  ^~~~~
 *			make: *** [Makefile:12: Empty.o] Error 1
 *		summary: 构造函数，和func()似乎都默认是private的，后面再尝试func()放在private的无参构造前面，
 *				使用object去调用一下试试;
 *		-------------------------------------------------------------------------------------------
 *
 * @output:
 *		书接上回(EMTPY_CLASS_WITH_NON_VIRTUAL_FUNCTIONS)，
 *		这次把func()放在了private的无参构造前面了，还是同样的报错，已经能够确定了;
 *
 *			$ make
 *			g++ -x c++ -std=c++11 -o Empty.o -c Empty.cpp
 *			Empty.cpp: In function ‘int main(int, char**)’:
 *			Empty.cpp:19:13: error: ‘void Empty::func()’ is private within this context
 *			   19 |  empty.func();
 *			      |
 *			Empty.cpp:6:7: note: declared private here
 *			    6 |  void func() {}
 *			      |       ^~~~
 *			make: *** [Makefile:12: Empty.o] Error 1
 *
 *
 * @summary: 没有在权限修饰符后面的functions，默认为private访问权限;
 */
