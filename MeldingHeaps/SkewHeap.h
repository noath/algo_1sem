#pragma once
#include <algorithm>
#include <assert.h>
#include "IHeap.h"

class SkewHeap :public IHeap {
	class Node {
		friend class SkewHeap;
		int key_;
		Node *left_;
		Node *right_;
		Node() {
			key_ = 0;
			left_ = nullptr;
			right_ = nullptr;
		}
		Node(const int &t) {
			key_ = t;
			left_ = nullptr;
			right_ = nullptr;
		}
		~Node() {}
	};

	Node *root_;

	Node *SubMeld(Node * first, Node * second) {
		if (first == nullptr)
			return second;
		if (second == nullptr)
			return first;

		if (first->key_ > second->key_)
			std::swap(first, second);

		first->right_ = SubMeld(first->right_, second);


		std::swap(first->right_, first->left_);
		return first;
	}

public:
	SkewHeap() {
		root_ = nullptr;
	}

	SkewHeap(const int &t) :root_(new Node(t)) {
	}

	void Meld(IHeap *other) {
		SkewHeap * second = dynamic_cast<SkewHeap*>(other);
		root_ = SubMeld(root_, second->root_);
	}

	void Add(const int &t) {
		root_ = SubMeld(root_, new Node(t));
	}

	int GetMin() {
		return root_->key_;
	}

	int ExtractMin() {
		Node * temp_node = SubMeld(root_->left_, root_->right_);
		int min_val = root_->key_;
		root_->left_ = root_->right_ = 0;
		delete root_;
		root_ = temp_node;
		return min_val;
	}
};