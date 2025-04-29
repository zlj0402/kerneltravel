/*
 * @brief: completed two versions of binary search -- porting on 29/4/2025
 * 			+ 1. while 循环的版本
 * 			+ 2. recursion version
 *
 * @output:
 * 		zlj@zlj-vm:~/zljgit/algorithm/C++/05_Binary_Search_Tree/01_Binary_Search$ ./main
 * 		1 2 2 3 4 6 7 7 8 8
 * 		r_idx: 9, target: 8
 *
 * 		binarySearch: idx: 8, value: 8
 * 		binarySearch2: idx: 8, value: 8
 * 
 * @description:
 * 		二分查找到的元素，在有多个重复元素时，元素个数变化的有序队列中查找，
 * 		idx 可能不是唯一的，但是值一定是匹配的;
 */
