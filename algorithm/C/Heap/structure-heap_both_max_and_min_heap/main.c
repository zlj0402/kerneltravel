#include "../RandomNumbers.h"
#include <iostream>
#define MAXN 1001
#define Rint register int
#define for(i, st, et) for(Rint i = st; i < et; i++)
using namespace std;

struct Node {
	int id;
	int val;
	int hid;
	int next;
}nodes[MAXN];
int n_idx;
int nodehash[MAXN];

Node* getOneNode(int id, int val) {
	Node* temp = &nodes[++n_idx];
	temp->id = id;
	temp->val = val;

	int h = id % MAXN;
	temp->next = nodehash[h];
	nodehash[h] = n_idx;
	return temp;
}

struct Heap {
	int N[2];
	Node* heap[2][MAXN];

	bool Cmp0(Node** heap, int i, int j) {
		return heap[i]->val > heap[j]->val || 
			(heap[i]->val == heap[j]->val && heap[i]->id < heap[j]->id);
	}

	bool Cmp1(Node* heap[], int i, int j) {
		return heap[i]->val < heap[j]->val || 
			(heap[i]->val == heap[j]->val && heap[i]->id > heap[j]->id);
	}

	void swap(Node* heap[], int i, int j) {
		Node* temp = heap[i];
		heap[i] = heap[j];
		heap[j] = temp;

		heap[i]->hid = i;
		heap[j]->hid = j;
	}

	void up(Node* heap[], int h, int idx) {
		bool (Heap::*Cmp)(Node**, int, int) = h == 0 ? &Heap::Cmp0 : &Heap::Cmp1;
		while (idx > 1 && (this->*Cmp)(heap, idx, idx >> 1)) {	// *Cmp 是堆类型对象的属性，即函数指针变量 // Cmp -> 类成员函数指针
			swap(heap, idx, idx >> 1);
			idx >>= 1;
		}
	}

	void down(Node* heap[], int h, int idx) {
		bool (Heap:: * Cmp)(Node**, int, int) = h == 0 ? &Heap::Cmp0 : &Heap::Cmp1;
		int child;
		while ((child = idx << 1) <= N[h]) {
			if (child < N[h] && (this->*Cmp)(heap, child + 1, child)) child++;
			if ((this->*Cmp)(heap, child, idx)) {
				swap(heap, child, idx);
				idx = child;
			}
			else return;
		}
	}

	void add(Node* heap[], int h, Node* one) {	// index in nodes[]
		heap[++N[h]] = one;
		one->hid = N[h];
		up(heap, h, N[h]);
	}

	void add(Node* one) {
		add(heap[0], 0, one);
		add(heap[1], 1, one);
	}

	void del(Node* heap[], int h, int pos) {
		swap(heap, N[h]--, pos);
		up(heap, h, pos);
		down(heap, h, pos);
	}

	void del(int pos) {
		del(heap[0], 0, pos);
		del(heap[1], 1, pos);
	}

	void mod(Node* heap[], int h, int pos) {
		up(heap, h, pos);
		down(heap, h, pos);
	}

	void mod(int pos) {
		mod(heap[0], 0, pos);
		mod(heap[1], 1, pos);
	}
}hq;

int main() {
	hq.N[0] = hq.N[1] = 0;

	int objectSize = 20;
	for (i, 0, objectSize) {
		int id = RandomNumbers::getRandomNumber(1, INT_MAX);
		int val = RandomNumbers::getRandomNumber(0, 10000);
		cout << "id: " << id << ", val: " << val << endl;
		Node* temp = getOneNode(id, val);
		hq.add(temp);
	}
	/*******************************************val-set and print done*******************************************/
	Node* arr[MAXN];
	int len[2] = { 0 };
	cout << "----------------------------------------------------------------------" << endl;
	cout << "hq.N[0]: " << hq.N[0] << ", hq.N[1]: " << hq.N[1] << endl;
	while (hq.N[0]) {
		cout << "id: " << hq.heap[0][1]->id << ", val: " << hq.heap[0][1]->val << endl;
		arr[len[0]++] = hq.heap[0][1];
		arr[len[1]++] = hq.heap[1][1];
		hq.del(1);
	}
	cout << "hq.N[0]: " << hq.N[0] << ", hq.N[1]: " << hq.N[1] << endl;
	cout << "--------------------------------heap del done--------------------------------------" << endl;


	/****************************************heap-del and print done*********************************************/
	for (i, 0, len[0]) hq.add(arr[i]);
	/******************************************val-readd done****************************************************/
	len[0] = len[1] = 0;
	cout << "--------------------------------heap readd done--------------------------------------" << endl;
	cout << "hq.N[0]: " << hq.N[0] << ", hq.N[1]: " << hq.N[1] << endl;
	while (hq.N[1]) {
		cout << "id: " << hq.heap[1][1]->id << ", val: " << hq.heap[1][1]->val << endl;
		arr[len[1]++] = hq.heap[1][1];
		hq.del(1);
	}
	cout << "hq.N[0]: " << hq.N[0] << ", hq.N[1]: " << hq.N[1] << endl;
	cout << "--------------------------------heap del done--------------------------------------" << endl;
}
