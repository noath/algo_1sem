#pragma once
#include <algorithm>
#include <assert.h>
#include "IHeap.h"

class LeftistHeap :public IHeap {

public:
	LeftistHeap() {
		root_ = nullptr;
	}
	LeftistHeap(int value) :root_(new Node(value)) {
	}
	void Meld(IHeap *other) {
		LeftistHeap *second = dynamic_cast<LeftistHeap*>(other);
		root_ = SubMeld(root_, second->root_);
	}
	void Add(const int &t) {
		root_ = SubMeld(root_, new Node(t));
	}
	int GetMin() {
		return root_->key_;
	}
	int ExtractMin() {
		Node *temp_node = SubMeld(root_->left_, root_->right_);
		int min_val = root_->key_;
		root_->left_ = root_->right_ = 0;
		delete root_;
		root_ = temp_node;
		return min_val;
	}
private:
	class Node {
		friend class LeftistHeap;
		int key_;
		Node *left_;
		Node *right_;
		int dist_;
		Node() {
			left_ = nullptr;
			right_ = nullptr;
			key_ = 0;
			dist_ = 1;
		}
		Node(const int &t) {
			left_ = nullptr;
			right_ = nullptr;
			key_ = t;
			dist_ = 1;
		}
		~Node() {}
	};

	int dist(Node *v) {
		if (v != nullptr)
			return v->dist_;
		else
			return 0;
	}
	Node *root_;
	Node *SubMeld(Node *first, Node *second) {
		if (first == nullptr)
			return second;
		if (second == nullptr)
			return first;
		if (first->key_ > second->key_)
			std::swap(first, second);
		first->right_ = SubMeld(first->right_, second);
		if (dist(first->right_) > dist(second->left_))
			std::swap(first->left_, first->right_);
		first->dist_ = dist(first->right_) + 1;
		return first;
	}
};