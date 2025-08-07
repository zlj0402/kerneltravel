/**
 * @brief: 内核中通过 list_entry & container_of 得到结构体的位置的方式，在 APP 中可以直接使用吗 -- 7/8/2025
 *
 * @output:
 * 		zlj@zlj-vm:~/zljgit/kerneltravel/00_Self_Attempt/linuxkernel/Include_List_H/Container_Of_In_App$ ./main
 * 		a addr: 0x7ffcd7c4f850
 * 		a.tList addr: 0x7ffcd7c4f858
 * 		list_entry(&a.tList, T_List, tList): 0x7ffcd7c4f850
 */
