/**
 * @brief: try to find out what size the empty class with more virtual functions has -- 24/3/2025
 * 
 * @output:
 * 		zlj@zlj-vm:~/zljgit/languages/C++/Textbook/Effective_C++/term05_Know_What_Functions_C++_Silently_Writes_And_Calls/Empty_Class_With_More_Virtual_Functions$ ./Empty
 * 		sizeof Empty class: 8
 * 		sizeof Empty class' object: 8
 *
 * @summary:
 * 		可以确定，不论具有多少个virtual函数，都只会使得class的object占据的空间多8bytes;
 * 		这个增量确定了，与virtual functions的个数的k=0的线性关系;
 *		
 *		那么是什么原因呢？为什么是8bytes呢？一定是8bytes吗？
 *		A:
 *			class具有virtual函数后，会多一个VPTR(virtual table pointer)虚函数指针，指向该类的虚函数表;
 *			其实就是多了一个指针的的大小;
 *			指针的大小，对于一个机器设备是固定的;
 *			64位的设备，一个指针是8bytes大小; 32位的设备，一个指针是4bytes大小;
 *		此时，该类已经不是真正的Empty class了，Empty class的定义，可以理解为:
 *			+ 没有非静态数据成员的类是“空类”。
 *			+ 但如果它包含虚函数、基类或其他特殊特性，它就可能不再是“真正的”空类了。
 *			
 */
