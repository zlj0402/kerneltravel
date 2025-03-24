/**
 * @brief: try to find out what size the empty class with virtual functions has -- 24/3/2025
 * 
 * @output:
 * 		zlj@zlj-vm:~/zljgit/languages/C++/Textbook/Effective_C++/term05_Know_What_Functions_C++_Silently_Writes_And_Calls/Empty_Class_With_Virtual_Functions$ ./Empty
 * 		sizeof Empty class: 8
 * 		sizeof Empty class' object: 8
 *
 * @summary:
 * 		可以初步看出来，具有虚函数，会使得class的object占据的空间变大;
 * 		那么，这种是k=1线性关系，还是k=0的线性关系?
 * 		见下回分晓;
 */
