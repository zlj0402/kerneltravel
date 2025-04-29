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
	/*
	bool operator>(Node a) {
		return val > a.val || (val == a.val && id < a.id);
	}

	bool operator<(Node a) {
		return val < a.val || (val == a.val && id > a.id);
	}*/
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
	int N, heap[MAXN];	// idx of nodes[] in heap[]

	bool Cmp0(int i, int j) {
		return nodes[heap[i]].val > nodes[heap[j]].val || 
			(nodes[heap[i]].val == nodes[heap[j]].val && nodes[heap[i]].id < nodes[heap[j]].id);
	}

	bool Cmp1(int i, int j) {
		return nodes[heap[i]].val < nodes[heap[j]].val || 
			(nodes[heap[i]].val == nodes[heap[j]].val && nodes[heap[i]].id < nodes[heap[j]].id);
	}

	void swap(int i, int j) {
		int temp = heap[i];
		heap[i] = heap[j];
		heap[j] = temp;

		nodes[heap[i]].hid = i;
		nodes[heap[j]].hid = j;
	}

	void up(int idx) {
		while (idx > 1 && Cmp1(idx, idx>>1)) {
			swap(idx, idx >> 1);
			idx >>= 1;
		}
	}

	void down(int idx) {
		int child;
		while ((child = idx << 1) <= N) {
			if (child < N && Cmp1(child+1, child)) child++;
			if (Cmp1(child, idx)) {
				swap(child, idx);
				idx = child;
			}
			else return;
		}
	}

	void add(int idx) {	// index in nodes[]
		heap[++N] = idx;
		nodes[idx].hid = N;
		up(N);
	}

	void del(int pos) {
		swap(N--, pos);
		up(pos);
		down(pos);
	}

	void mod(int pos) {
		up(pos);
		down(pos);
	}
}hq;

int main() {
	int objectSize = 20;
	for (i, 0, objectSize) {
		int id = RandomNumbers::getRandomNumber(1, INT_MAX);
		int val = RandomNumbers::getRandomNumber(0, 10000);
		cout << "id: " << id << ", val: " << val << endl;
		Node* temp = getOneNode(id, val);
		hq.add(n_idx);
	}
	/*******************************************val-set and print done*******************************************/
	int arr[MAXN];
	int len = 0;
	cout << "----------------------------------------------------------------------" << endl;
	while (hq.N) {
		cout << "id: " << nodes[hq.heap[1]].id << ", val: " << nodes[hq.heap[1]].val << endl;
		arr[len++] = hq.heap[1];
		hq.del(1);
	}
	/****************************************heap-del and print done*********************************************/
	for (i, 0, len) hq.add(arr[i]);
	/******************************************val-readd done****************************************************/

	cout << "----------------------------------------------------------------------" << endl;
	while (hq.N) {
		cout << "id: " << nodes[hq.heap[1]].id << ", val: " << nodes[hq.heap[1]].val << endl;
		arr[len++] = hq.heap[1];
		hq.del(1);
	}
}
