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
zlj@zlj-vm:~/zljgit/algorithm/C++/Sorting_Advance/All_Sort_Types_Test$ ./main
Test for random array, size = 1000000, random range [0, 1000000]
Shell Sort : 0.258336 s
Shell Sort2 : 0.246151 s
Merge Sort : 0.323101 s
Merge Sort2 : 0.275649 s
Merge Sort BU : 0.284074 s
Quick Sort3 : 0.117249 s
Quick Sort4 : 0.176891 s

Test for more ordered random array, size = 1000000, random range [0, 3]
Shell Sort : 0.074001 s
Shell Sort2 : 0.054071 s
Merge Sort : 0.120813 s
Merge Sort2 : 0.093494 s
Merge Sort BU : 0.111141 s
Quick Sort3 : 0.076152 s
Quick Sort4 : 0.014331 s

Test for nearly ordered array, size = 1000000, swap time = 100
Shell Sort : 0.093534 s
Shell Sort2 : 0.081719 s
Merge Sort : 0.18656 s
Merge Sort2 : 0.067945 s
Merge Sort BU : 0.070476 s
Quick Sort3 : 0.042153 s
Quick Sort4 : 0.11241 s

Test for ascending array, size = 1000000
Shell Sort : 0.055894 s
Shell Sort2 : 0.034646 s
Merge Sort : 0.184805 s
Merge Sort2 : 0.003265 s
Merge Sort BU : 0.003217 s
Quick Sort3 : 0.042879 s
Quick Sort4 : 0.107745 s

Test for desending array, size = 1000000
Shell Sort : 0.081537 s
Shell Sort2 : 0.049866 s
Merge Sort : 0.214362 s
Merge Sort2 : 0.119672 s
Merge Sort BU : 0.144022 s
Quick Sort3 : 0.046456 s
Quick Sort4 : 0.122085 s
*/
