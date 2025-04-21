/*
 * @brief: all versions of each sort algorithm && their comparation -- final update: 21/4/2025
 *
 * @output: + please update after add a version under the sort type;
 * 			+ had better sort by time efficientcy
 * 				+ such as: SelectionSort -> SelectionSort2 -> ... -> SelectionSortn
 * 					SelectionSortn -> is the best version of Selection Sort
 * 			+ output as following:
 */

/* output:
Test for random array, size = 10000000, random range [0, 10000000]
Shell Sort : 3.817 s
Shell Sort2 : 3.671 s
Merge Sort : 4.84 s
Merge Sort2 : 4.786 s
Merge Sort BU : 3.704 s
Quick Sort3 : 1.185 s
Quick Sort4 : 1.571 s
Heap Sort : 4.086 s
Heap Sort2 : 3.918 s
Heap Sort3 : 3.522 s

Test for more ordered random array, size = 10000000, random range [0, 3]
Shell Sort : 1.138 s
Shell Sort2 : 0.688 s
Merge Sort : 4.353 s
Merge Sort2 : 4.247 s
Merge Sort BU : 3.134 s
Quick Sort3 : 0.883 s
Quick Sort4 : 0.181 s
Heap Sort : 1.197 s
Heap Sort2 : 1.153 s
Heap Sort3 : 1.029 s

Test for nearly ordered array, size = 10000000, swap time = 100
Shell Sort : 0.712 s
Shell Sort2 : 0.426 s
Merge Sort : 4.045 s
Merge Sort2 : 4.072 s
Merge Sort BU : 0.04 s
Quick Sort3 : 7.06 s
Quick Sort4 : 17.604 s
Heap Sort : 1.963 s
Heap Sort2 : 1.461 s
Heap Sort3 : 1.322 s

Test for ascending array, size = 10000000
Shell Sort : 0.709 s
Shell Sort2 : 0.434 s
Merge Sort : 3.995 s
Merge Sort2 : 3.842 s
Merge Sort BU : 0.034 s
Quick Sort3 : 6.807 s
Quick Sort4 : 18.696 s
Heap Sort : 1.833 s
Heap Sort2 : 1.419 s
Heap Sort3 : 1.32 s

Test for desending array, size = 10000000
Shell Sort : 1.022 s
Shell Sort2 : 0.712 s
Merge Sort : 5.516 s
Merge Sort2 : 4.24 s
Merge Sort BU : 4.144 s
Quick Sort3 : 11.412 s
Quick Sort4 : 31.543 s
Heap Sort : 1.827 s
Heap Sort2 : 1.898 s
Heap Sort3 : 1.495 s
*/
