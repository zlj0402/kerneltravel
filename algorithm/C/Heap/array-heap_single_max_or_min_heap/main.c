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
Node* heap[MAXN];
int hSize;

/*
bool Cmp(int i, int j) {
	return heap[i]->val > heap[j]->val || 
		(heap[i]->val == heap[j]->val && heap[i]->id < heap[j]->id);
}*/

bool Cmp(int i, int j) {
	return heap[i]->val < heap[j]->val ||
		(heap[i]->val == heap[j]->val && heap[i]->id > heap[i]->id);
}

void heapSwap(int i, int j) {
	Node* temp = heap[i];
	heap[i] = heap[j];
	heap[j] = temp;

	heap[i]->hid = i;
	heap[j]->hid = j;
}

int upHeapify(int idx){
	while (idx > 1) {
		if (Cmp(idx, idx >> 1)) {
			heapSwap(idx, idx >> 1);
			idx >>= 1;
		}
		else break;
	}
	return idx;
}

void downHeapify(int idx) {
	int child;
	while ((child = idx << 1) <= hSize) {
		if (child < hSize && Cmp(child+1, child)) child++;	//  这里竟然写成了Cmp(child, idx)，虽然理解了堆得的每个主要过程；主要凭借记忆写时，还是偶尔犯点错
		if (Cmp(child, idx)) {
			heapSwap(child, idx);
			idx = child;
		}
		else return;
	}
}

void heapAdd(Node* one) {
	heap[++hSize] = one;
	one->hid = hSize;
	upHeapify(hSize);
}

void heapDel(int pos) {
	heapSwap(hSize--, pos);
	upHeapify(pos);
	downHeapify(pos);
}

void heapMod(int pos) {
	upHeapify(pos);
	downHeapify(pos);
}
/*************************************heap structure done****************************************/

int main() {
	hSize = 0;
	n_idx = 0;

	static int objectSize = 100;
	for (Rint i = 0; i < objectSize; i++) {
		int id = RandomNumbers::getRandomNumber(1, INT_MAX);
		int val = RandomNumbers::getRandomNumber(0, 20000);
		Node* temp = getOneNode(id, val);
		cout << "id: " << id << ", val: " << val << endl;
		heapAdd(temp);
	}
	/********************************array-set done*************************************/
	cout << "----------------------------------------------------------" << endl;
	Node** arr = new Node*[objectSize];
	int len = 0;
	while (hSize) {
		cout << "id: " << heap[1]->id << ", val: " << heap[1]->val << endl;
		arr[len++] = heap[1];
		heapDel(1);
	}
	/************************************heap-del and print*********************************/
	for (Rint i = 0; i < len; i++) {
		heapAdd(arr[i]);
	}
	/*************************************heap-readd done*************************************/
	cout << "----------------------------------------------------------" << endl;
	len = 0;
	while (hSize) {
		cout << "id: " << heap[1]->id << ", val: " << heap[1]->val << endl;
		arr[len++] = heap[1];
		heapDel(1);
	}
	delete[] arr;
}
