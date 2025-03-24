/**
 * @brief: try to find out: If a class has a custom ctor, will it still have a default no-argument ctor -- 24/3/2025
 * 
 * @output:
 * 		after `make` command:
 * 			zlj@zlj-vm:~/zljgit/languages/C++/Textbook/Effective_C++/Part2_Constructors_Destructors_And_AssignmentOperators/term05_Know_What_Functions_C++_Silently_Writes_And_Calls/Class_With_Customed_CTOR_How_About_Default$ make
 * 			g++ -x c++ -std=c++11 -o Empty.o -c Empty.cpp
 * 			Empty.cpp: In function ‘int main(int, char**)’:
 * 			Empty.cpp:16:8: error: no matching function for call to ‘Empty::Empty()’
 * 			   16 |  Empty empty;
 * 			      |        ^~~~~
 * 			Empty.cpp:9:3: note: candidate: ‘Empty::Empty(char)’
 * 			    9 |   Empty(char dummy) {}
 * 			      |   ^~~~~
 * 			Empty.cpp:9:3: note:   candidate expects 1 argument, 0 provided
 * 			Empty.cpp:5:7: note: candidate: ‘constexpr Empty::Empty(const Empty&)’
 * 			    5 | class Empty {
 * 			      |       ^~~~~
 * 			Empty.cpp:5:7: note:   candidate expects 1 argument, 0 provided
 * 			Empty.cpp:5:7: note: candidate: ‘constexpr Empty::Empty(Empty&&)’
 * 			Empty.cpp:5:7: note:   candidate expects 1 argument, 0 provided
 * 			make: *** [Makefile:12: Empty.o] Error 1
 *
 * @description:
 * 		从给出的提示看出，当执行到Emtpy emtpy;
 * 		编译器是去查找有没有匹配的Empty::empty()，而编译器显示没有该构造函数，
 * 		这说明了，@summary
 *
 * @summary:
 * 		当我们自定义了一个构造函数之后，编译器不再会为我们生成默认的无参构造函数;
 */
