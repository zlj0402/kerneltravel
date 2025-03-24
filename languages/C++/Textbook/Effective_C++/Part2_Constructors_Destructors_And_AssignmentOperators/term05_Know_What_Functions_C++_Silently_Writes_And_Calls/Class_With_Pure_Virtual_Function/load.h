/**
 * @brief: try to find out whether one class which has pure virtual function(s) can generate its objects or not -- 24/3/2025
 * 
 * @output:
 * 		zlj@zlj-vm:~/zljgit/languages/C++/Textbook/Effective_C++/Part2_Constructors_Destructors_And_AssignmentOperators/term05_Know_What_Functions_C++_Silently_Writes_And_Calls/Class_With_Pure_Virtual_Function$ make
 * 		g++ -x c++ -std=c++11 -o Empty.o -c Empty.cpp
 * 		Empty.cpp: In function ‘int main(int, char**)’:
 * 		Empty.cpp:21:8: error: cannot declare variable ‘empty’ to be of abstract type ‘Empty’
 * 		   21 |  Empty empty;
 * 		      |        ^~~~~
 * 		Empty.cpp:5:7: note:   because the following virtual functions are pure within ‘Empty’:
 * 		    5 | class Empty {
 * 		      |       ^~~~~
 * 		Empty.cpp:11:15: note:  ‘virtual void Empty::virFunc2()’
 * 		   11 |  virtual void virFunc2() = 0
 *		      |               ^~~~~~~~
 *		make: *** [Makefile:12: Empty.o] Error 1
 *
 * @description:
 * 		直接拿前面的Empty class做现成的使用，任何class带上纯虚函数，都是一样的;
 *
 * @summary:
 * 		cannot declare variable ‘empty’ to be of abstract type ‘Empty’ => 带有纯虚函数的class，被认作是抽象的class，
 * 		可以看出，抽象的class是不能够声明自己的object;
 */
