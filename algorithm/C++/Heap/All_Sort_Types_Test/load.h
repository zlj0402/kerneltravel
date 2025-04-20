/*
 * @brief: all versions of each sort algorithm && their comparation
 *
 * @output: + please update after add a version under the sort type;
 * 			+ had better sort by time efficientcy
 * 				+ such as: SelectionSort -> SelectionSort2 -> ... -> SelectionSortn
 * 					SelectionSortn -> is the best version of Selection Sort
 * 			+ output as following:
 */

/* output:
zlj@zlj-vm:~/zljgit/algorithm/C++/Heap/All_Sort_Types_Test$ ./main
Test for random array, size = 1000000, random range [0, 1000000]
Shell Sort : 0.258589 s
Shell Sort2 : 0.243444 s
Merge Sort : 0.33557 s
Merge Sort2 : 0.291954 s
Merge Sort BU : 0.283711 s
Quick Sort3 : 0.114976 s
Quick Sort4 : 0.21521 s
Heap Sort : 0.371033 s
Heap Sort2 : 0.316702 s

Test for more ordered random array, size = 1000000, random range [0, 3]
Shell Sort : 0.077147 s
Shell Sort2 : 0.054327 s
Merge Sort : 0.125998 s
Merge Sort2 : 0.126637 s
Merge Sort BU : 0.114938 s
Quick Sort3 : 0.075177 s
Quick Sort4 : 0.016356 s
Heap Sort : 0.100353 s
Heap Sort2 : 0.094329 s

Test for nearly ordered array, size = 1000000, swap time = 100
Shell Sort : 0.096635 s
Shell Sort2 : 0.083452 s
Merge Sort : 0.189684 s
Merge Sort2 : 0.134352 s
Merge Sort BU : 0.071581 s
Quick Sort3 : 0.043347 s
Quick Sort4 : 0.163972 s
Heap Sort : 0.273544 s
Heap Sort2 : 0.1473 s

Test for ascending array, size = 1000000
Shell Sort : 0.052976 s
Shell Sort2 : 0.03472 s
Merge Sort : 0.186992 s
Merge Sort2 : 0.130597 s
Merge Sort BU : 0.002342 s
Quick Sort3 : 0.046599 s
Quick Sort4 : 0.16256 s
Heap Sort : 0.338786 s
Heap Sort2 : 0.153072 s

Test for desending array, size = 1000000
Shell Sort : 0.088067 s
Shell Sort2 : 0.050039 s
Merge Sort : 0.217475 s
Merge Sort2 : 0.131169 s
Merge Sort BU : 0.131018 s
Quick Sort3 : 0.046445 s
Quick Sort4 : 0.170373 s
Heap Sort : 0.242856 s
Heap Sort2 : 0.150232 s
*/
