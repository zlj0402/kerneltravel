/**
 * @brief: try to know what size does a class or a class object with no data members but with not-virtual functions occupy -- 23/3/2025
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
 *		还做首要的，@brief
 *
 * @output:
 * 		zlj@zlj-vm:~/zljgit/languages/C++/Textbook/Effective_C++/term05_Know_What_Functions_C++_Silently_Writes_And_Calls/Empty_Class_With_Non_Virtual_Functions$ ./Empty
 * 		sizeof Empty class: 1
 * 		sizeof Empty class' object: 1
 *
 * @summary:
 * 		可见不论拥有多少个，非虚函数，并不会使得类对象所占据的空间增加
 */
