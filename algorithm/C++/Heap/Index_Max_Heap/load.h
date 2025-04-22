/*
 * @brief: add index max heap -- porting on 22/4/2025
 *
 * @description:
 * 		+ 申请了等同 data[] 数组元素个数的长度的 indexes[] 和 reverse[];
 * 			+ indexes[]: 真正堆排数组; 
 * 				+ 每个 indexes[] 值，记录的是 data 在 data[] 的idx;
 * 				+ 比较是根据 data[indexes[i]] 的值;
 * 			+ reverse[]: 记录 data 在 indexes 中的下标;
 *		+ data[] indexes[] reverse[] 数组，都是从 idx 1 的位置，进行存储;
 *		+ insert 函数:
 *			// 传入的 i 对用户而言，是从 0 索引的; 在 data[] 中，用户视角的下标
 *			void insert(int i, Item item) {
 *
 *				assert(count + 1 <= capacity);
 *				assert(i + 1 >= 1 && i + 1 <= capacity);
 *
 *				// 再插入一个新元素前,还需要保证索引i所在的位置是没有元素的。
 *				assert(!contains(i));
 *
 *				data[++i] = item;   // ++i 是为了用户视角的 i 在 Heap 当中真正起始的位置要，右偏移1
 *				indexes[++count] = i;
 *				reverse[i] = count; // 记录 item 元素在 indexes 中的位置，虽然后面马上就可能在 shiftUp 中改变;
 *
 *				shiftUp(count);
 *			}
 *			+ param: int i -> 用户视角，从 idx 开始的;
 */
