//
// 2023/08/12 复习一下，堆的数组实现
//

#include "../RandomNumbers.h"
#include <iostream>
#define MAXN 1001
#define Rint register int
using namespace std;

struct Node {
	int id;
	int val;
	int hid;
	int next;
}nodes[MAXN];
int n_idx;

int nodeHash[MAXN];
/************************************structure done**********************************/

Node* getOneNode(int id, int val) {
	Node* temp = &nodes[++n_idx];
	temp->id = id;
	temp->val = val;
	temp->hid = 0;

	int h = id % MAXN;
	temp->next = nodeHash[h];
	nodeHash[h] = n_idx;
	return temp;
}

int findNode(int id) {
	for (Rint i = nodeHash[id % MAXN]; i; i = nodes[i].next) {
		if (nodes[i].id == id)
			return i;
	}
	return 0;
}
/************************************Util done*******************************************/
Node* heap[2][MAXN];
int hSize[2];


bool Cmp0(Node* heap[], int i, int j) {
	return heap[i]->val > heap[j]->val ||
		(heap[i]->val == heap[j]->val && heap[i]->id < heap[j]->id);
}

bool Cmp1(Node* heap[], int i, int j) {
	return heap[i]->val < heap[j]->val ||
		(heap[i]->val == heap[j]->val && heap[i]->id > heap[i]->id);
}

void heapSwap(Node* heap[], int i, int j) {
	Node* temp = heap[i];
	heap[i] = heap[j];
	heap[j] = temp;

	heap[i]->hid = i;
	heap[j]->hid = j;
}

int upHeapify(Node* heap[], int h, int idx) {
	bool (*Cmp)(Node**, int, int) = h == 0 ? Cmp0 : Cmp1;
	while (idx > 1) {
		if (Cmp(heap, idx, idx >> 1)) {
			heapSwap(heap, idx, idx >> 1);
			idx >>= 1;
		}
		else break;
	}
	return idx;
}

void downHeapify(Node* heap[], int h, int idx) {
	bool (*Cmp)(Node**, int, int) = h == 0 ? Cmp0 : Cmp1;
	int child;
	while ((child = idx << 1) <= hSize[h]) {
		if (child < hSize[h] && Cmp(heap, child + 1, child)) child++;	//  这里竟然写成了Cmp(child, idx)，虽然理解了堆得的每个主要过程；主要凭借记忆写时，还是偶尔犯点错
		if (Cmp(heap, child, idx)) {
			heapSwap(heap, child, idx);
			idx = child;
		}
		else return;
	}
}

void heapAdd(Node* heap[], int h, Node* one) {
	heap[++hSize[h]] = one;
	one->hid = hSize[h];
	upHeapify(heap, h, hSize[h]);
}

void heapAdd(Node* one) {
	heapAdd(heap[0], 0, one);
	heapAdd(heap[1], 1, one);
}

void heapDel(Node* heap[], int h, int pos) {
	heapSwap(heap, hSize[h]--, pos);
	upHeapify(heap, h, pos);
	downHeapify(heap, h, pos);
}

void heapDel(int pos) {
	heapDel(heap[0], 0, pos);
	heapDel(heap[1], 1, pos);
}

void heapMod(Node* heap[], int h, int pos) {
	upHeapify(heap, h, pos);
	downHeapify(heap, h, pos);
}

void heapMod(int pos) {
	heapMod(heap[0], 0, pos);
	heapMod(heap[1], 1, pos);
}
/*************************************heap structure done****************************************/

int main() {
	hSize[0] = hSize[1] = 0;
	n_idx = 0;

	static int objectSize = 20;
	for (Rint i = 0; i < objectSize; i++) {
		int id = RandomNumbers::getRandomNumber(1, INT_MAX);
		int val = RandomNumbers::getRandomNumber(0, 20000);
		Node* temp = getOneNode(id, val);
		cout << "id: " << id << ", val: " << val << endl;
		heapAdd(temp);
	}
	/********************************array-set done*************************************/
	cout << "-------------------------------array-set done---------------------------" << endl;
	cout << "hSize[0]: " << hSize[0] << ", hSize[1]: " << hSize[1] << endl;
	Node** arr = new Node * [objectSize];
	int len = { 0 };
	while (hSize[0]) {
		cout << "id: " << heap[0][1]->id << ", val: " << heap[0][1]->val << endl;
		arr[len++] = heap[0][1];
		heapDel(1);
	}
	cout << "hSize[0]: " << hSize[0] << ", hSize[1]: " << hSize[1] << endl;
	/************************************heap-del and print*********************************/
	cout << "-------------------------------heap-del and print---------------------------" << endl;
	for (Rint i = 0; i < len; i++) {
		heapAdd(arr[i]);
	}
	/*************************************heap-readd done*************************************/
	cout << "--------------------------------heap-readd done--------------------------" << endl;
	cout << "hSize[0]: " << hSize[0] << ", hSize[1]: " << hSize[1] << endl;
	len = 0;
	while (hSize[1]) {
		cout << "id: " << heap[1][1]->id << ", val: " << heap[1][1]->val << endl;
		arr[len++] = heap[1][1];
		heapDel(1);
	}
	cout << "hSize[0]: " << hSize[0] << ", hSize[1]: " << hSize[1] << endl;
	delete[] arr;
}
