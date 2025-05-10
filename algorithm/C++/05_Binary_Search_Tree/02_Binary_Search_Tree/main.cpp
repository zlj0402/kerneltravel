//
// finished by liangj.zhang on 10/5/2025, porting on 10/5/2025, last updated on 10/5/2025
// updated on 10/5/2025: cp remove from liuyubobobo's git, find minimum node from right child tree
// 							GitHub URL: https://github.com/liuyubobobo/Play-with-Algorithms/blob/master/05-Binary-Search-Tree/Course%20Code%20(C%2B%2B)/08-Binary-Search-Tree-Remove/main.cpp
//

#include <iostream>
#include <vector>
#include <queue>
#include <cassert>
#include "FileOps.h"

using namespace std;

template<typename Key, typename Value>
class BST {

private:
	struct Node {

		Key key;
		Value value;
		Node* left;
		Node* right;

		Node(Key key, Value value) : key(key), value(value), left(nullptr), right(nullptr) {}

		Node(Node* rhs) : key(rhs->key), value(rhs->value), left(rhs->left), right(rhs->right) {}
	};

	Node* root;
	int count;

public:
	BST() : root(nullptr), count(0) {}

	~BST() {
		destroy(root);
	}

	int getSize() const {
		return count;
	}

	bool isEmpty() const {
		return count == 0;
	}

	void insert(Key key, Value value) {

		// 1. 方式一
		//root = insert(root, key, value);
		// 2. 方式二
		__insert(key, value);
	}

	// 查看二分搜索树中是否存在键key
	bool contains(Key key) {
		return contains(root, key);
	}

	// 在二分搜索树中搜索键key所对应的值。如果这个值不存在, 则返回NULL
	Value* search(Key key) {
		return search(root, key);
	}

	// 前序遍历
	void preOrder() {
		preOrder(root);
	}

	// 中序遍历
	void inOrder() {
		inOrder(root);
	}

	// 后续遍历
	void postOrder() {
		postOrder(root);
	}

	// 层序遍历
	void levelOrder() {

		if (!root) return;

		queue<Node*> q;
		q.push(root);

		while (!q.empty()) {

			Node* cur = q.pop();
			if (cur->left)
				q.push(cur->left);
			if (cur->right)
				q.push(cur->right);

			cout << cur->key << endl;
		}
	}

	// 寻找 BST 当中最小的键值
	Key minimum() {

		assert(count != 0);
		Node* minNode = minimum(root);
		return minNode->key;
	}

	// 寻找 BST 当中最大的键值
	Key maximum() {

		assert(count != 0);
		Node* maxNode = maximum(root);
		return maxNode->key;
	}

	// 从二叉搜索树当中，删除最小值所在节点
	void removeMin() {
		if (root)
			root = removeMin(root);
	}

	// 从二叉搜索树当中，删除最大值所在节点
	void removeMax() {
		if (root)
			root = removeMax(root);
	}

	// 从二叉树搜索树中，删除键值为 key 的节点
	void remove(Key key) {

		// 1. 方式一
		root = remove(root);
		// 2. 方式二
		//root = remove2(root);
	}

private:
	// 向以 node 为根的二叉搜索树中，插入节点(key, value)
	// 返回插入新节点后的二叉搜索树的根
	Node* insert(Node* node, Key key, Value value) {

		if (node == nullptr) {
			count++;
			return new Node(key, value);
		}

		if (key == node->key)
			node->value = value;
		else if (key < node->key)
			node->left = insert(node->left, key, value);
		else // key > node->key
			node->right = insert(node->right, key, value);

		return node;
	}

	// 非递归方式 insert 一个新节点
	void __insert(Key key, Value value) {

		if (!root) {

			root = new Node(key, value);
			count++;
			return;
		}

		Node* parent = nullptr;
		Node* cur = root;
		while (cur) {

			if (cur->key == key) {
				cur->value = value;
				return;
			}

			parent = cur;
			if (cur->key > key)
				cur = cur->left;
			else
				cur = cur->right;
		}

		count++;
		if (parent->key > key)
			parent->left = new Node(key, value);
		else
			parent->right = new Node(key, value);
	}

	// 查看以node为根的二分搜索树中是否包含键值为key的节点, 使用递归算法
	bool contains(Node* node, Key key) {

		if (!node)
			return false;

		if (node->key == key)
			return true;
		else if (node->key > key)
			return contains(node->left, key);
		else	// node->key < key
			return contains(node->right, key);
	}

	// 在以node为根的二分搜索树中查找key所对应的value, 递归算法
	// 若value不存在, 则返回NULL
	Value* search(Node* node, Key key) {

		if (!node)
			return nullptr;

		if (node->key == key)
			return &(node->value);
		else if (node->key > key)
			return search(node->left, key);
		else	// node->key < key
			return search(node->right, key);
	}

	// 对以node为根的二分搜索树进行前序遍历, 递归算法
	void preOrder(Node* node) {

		if (node) {

			cout << node->key << endl;
			preOrder(node->left);
			preOrder(node->right);
		}
	}

	// 对以node为根的二分搜索树进行中序遍历, 递归算法
	void inOrder(Node* node) {

		if (node) {

			inOrder(node->left);
			cout << node->key << endl;
			inOrder(node->right);
		}
	}

	// 对以node为根的二分搜索树进行后序遍历, 递归算法
	void postOrder(Node* node) {

		if (node) {

			postOrder(node->left);
			postOrder(node->right);
			cout << node->key << endl;
		}
	}

	// 释放以node为根的二分搜索树的所有节点
	// 采用后续遍历的递归算法
	void destroy(Node* node) {

		if (node) {

			destroy(node->left);
			destroy(node->right);
			delete node;
			count--;
		}
	}

	// 在以 node 为根的 BST 中，返回最小键值的节点
	Node* minimum(Node* node) {

		if (!node->left)
			return node;

		return minimum(node->left);
	}

	// 在以 node 为根的 BST 中，返回最大键值的节点
	Node* maximum(Node* node) {

		if (!node->right)
			return node;

		return maximum(node->right);
	}

	// 删除掉以 node 为根的二叉搜索树中的最小节点
	// 返回删除节点后新的二叉搜索树的根
	Node* removeMin(Node* node) {

		if (node->left == nullptr) {

			Node* rightNode = node->right;
			delete node;
			count--;
			return rightNode;
		}

		node->left = removeMin(node->left);
		return node;
	}

	// 删除掉以 node 为根的二叉搜索树的最大节点
	// 返回删除节点后新的二叉搜索树
	Node* removeMax(Node* node) {

		if (node->right == nullptr) {

			Node* leftNode = node->left;
			delete node;
			count--;
			return leftNode;
		}

		node->right = removeMax(node->right);
		return node;
	}

	// 删除掉以 node 为根的二分搜索树中键值为 key 的节点
	// 返回删除节点后新的二分搜索树的根
	Node* remove(Node* node, Key key) {	// 时间复杂度：O(logn)

		if (!node)
			return nullptr;

		if (key < node->key) {

			node->left = remove(node->left, key);
			return node;	// 原来错误的写法：漏掉了
		}
		else if (key > node->key) {

			node->right = remove(node->right, key);
			return node;	// 原来错误的写法：漏掉了
		}
		else {	// key == node->key
			// 此次方式，是找到左子树最大的节点，替换删除节点的位置
			if (!node->left) {

				Node* rightNode = node->right;
				delete node;
				count--;
				return rightNode;
			}
			else if (!node->right) {

				Node* leftNode = node->left;
				delete node;
				count--;
				return leftNode;
			}
			else {	// node->left != nullptr && node->right != nullptr

				Node* successor = new Node(maximum(node->left));
				successor->right = node->right;
				successor->left = removeMax(node->left);
				count++;

				delete node;
				count--;

				return successor;
			}
		}

	}

	// 删除掉以node为根的二分搜索树中的最大节点, 递归算法
	// 返回删除节点后新的二分搜索树的根
	Node* removeMax(Node* node) {

		if (node->right == NULL) {

			Node* leftNode = node->left;
			delete node;
			count--;
			return leftNode;
		}

		node->right = removeMax(node->right);
		return node;
	}

	// 删除掉以node为根的二分搜索树中键值为key的节点, 递归算法
	// 返回删除节点后新的二分搜索树的根
	Node* remove2(Node* node, Key key) {

		if (node == NULL)
			return NULL;

		if (key < node->key) {
			node->left = remove(node->left, key);
			return node;
		}
		else if (key > node->key) {
			node->right = remove(node->right, key);
			return node;
		}
		else {   // key == node->key

			// 待删除节点左子树为空的情况
			if (node->left == NULL) {
				Node* rightNode = node->right;
				delete node;
				count--;
				return rightNode;
			}

			// 待删除节点右子树为空的情况
			if (node->right == NULL) {
				Node* leftNode = node->left;
				delete node;
				count--;
				return leftNode;
			}

			// 此次方式，是找到右子树最小的节点，替换删除节点的位置
			//
			// 待删除节点左右子树均不为空的情况

			// 找到比待删除节点大的最小节点, 即待删除节点右子树的最小节点
			// 用这个节点顶替待删除节点的位置
			Node* successor = new Node(minimum(node->right));
			count++;

			successor->right = removeMin(node->right);
			successor->left = node->left;

			delete node;
			count--;

			return successor;
		}
	}
};

int main() {

	// 使用圣经作为我们的测试用例
	string filename = "bible.txt";
	vector<string> words;
	if (FileOps::readFile(filename, words)) {

		cout << "There are totally " << words.size() << " words in " << filename << endl;
		cout << endl;


		// 测试 BST
		time_t startTime = clock();

		// 统计圣经中所有词的词频
		// 注: 这个词频统计法相对简陋, 没有考虑很多文本处理中的特殊问题
		// 在这里只做性能测试用
		BST<string, int> bst = BST<string, int>();
		for (vector<string>::iterator iter = words.begin(); iter != words.end(); iter++) {
			int* res = bst.search(*iter);
			if (res == NULL)
				bst.insert(*iter, 1);
			else
				(*res)++;
		}

		// 输出圣经中god一词出现的频率
		if (bst.contains("gods"))
			cout << "'gods' : " << *bst.search("gods") << endl;
		else
			cout << "No word 'gods' in " << filename << endl;

		time_t endTime = clock();

		cout << "BST , time: " << double(endTime - startTime) / CLOCKS_PER_SEC << " s." << endl;
		cout << endl;


		// 测试顺序查找表 SST
		startTime = clock();

		//// 统计圣经中所有词的词频
		//// 注: 这个词频统计法相对简陋, 没有考虑很多文本处理中的特殊问题
		//// 在这里只做性能测试用
		//SequenceST<string, int> sst = SequenceST<string, int>();
		//for (vector<string>::iterator iter = words.begin(); iter != words.end(); iter++) {
		//	int* res = sst.search(*iter);
		//	if (res == NULL)
		//		sst.insert(*iter, 1);
		//	else
		//		(*res)++;
		//}

		//// 输出圣经中god一词出现的频率
		//if (sst.contain("god"))
		//	cout << "'god' : " << *sst.search("god") << endl;
		//else
		//	cout << "No word 'god' in " << filename << endl;

		//endTime = clock();

		//cout << "SST , time: " << double(endTime - startTime) / CLOCKS_PER_SEC << " s." << endl;
	}

	return 0;
}
