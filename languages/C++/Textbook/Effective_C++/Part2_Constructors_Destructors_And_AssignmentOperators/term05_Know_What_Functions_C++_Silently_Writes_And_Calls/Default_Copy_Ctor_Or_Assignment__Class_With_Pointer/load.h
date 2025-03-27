/**
 * @brief: try to find out if one object(its class with pointer member) which is created by default copy ctor, what does the object's pointer member point to -- 27/3/2025
 *
 * @output:
 * 		zlj@zlj-vm:~/zljgit/languages/C++/Textbook/Effective_C++/Part2_Constructors_Destructors_And_AssignmentOperators/term05_Know_What_Functions_C++_Silently_Writes_And_Calls/Default_Copy_Ctor_Or_Assignment__Class_With_Pointer$ ./NamedObject
 * 		ctor called. --- 140724805538696
 * 		*(no1.objectPointer): 10
 *
 * 		copy ctor: NamedObject<int> no2(no1);
 * 		*(no2.objectPointer): 10
 *
 * 		*(no2.objectPointer): 20
 *
 * 		How about a and *(no1.objectPointer)?
 * 		a: 20
 * 		*(no1.objectPointer): 20
 * 		dtor called. --- 140724805538704
 * 		free(): invalid pointer
 * 		Aborted (core dumped)
 *
 * @description:
 * 		结合程序看，main函数的一开始，创建了a = 10; 并以a的值创建了no1;
 * 		所以no1.getVal()输出为 10;
 *
 * 		no2是默认的拷贝构造的方式创建的;(结合后面，能推定no2的地址为140724805538704)
 * 		发现指针指向的地方的值，也是10;
 * 		(那是no2的pointer也指向a，还是no2的pointer指向新的地方，值也为10呢？)
 *
 * 		通过no2.setVal(20)修改了，no2的pointer指向的地方的值，现在为20;
 * 		在看看a和*(no1.objectPointer)值，
 * 		我们现在可以推定，默认的拷贝构造，就是单纯的一字节一字节的拷贝，所以指针变量的值，也是拷贝过来的，和拷贝构造参数变量的pointer存储的地址是相同的;
 *
 * 		+ free(): invalid pointer，为什么会有这个错误?
 * 			因为free()试图去free一个非new的int a;
 *
 * 		+ 为什么能推定140724805538704是no2的地址?
 * 			NamedObject class的大小，只有一个指针的大小，64位机器的指针大小为8字节，
 * 			并且no2的创建顺序，紧跟no1的后面，
 * 			140724805538704 - 140724805538696 = 8
 *
 * @summary:
 * 		默认的拷贝构造，是一字节一字节的复制过来的;包括指针变量存储的地址;
 */
