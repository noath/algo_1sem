#pragma once
#include <iostream>
#include <algorithm>
#include <memory.h>
#include <set>
#include <iterator>
#include <vector>
#include "IHeap.h"

class BinomialHeap : public IHeap {
public:
	BinomialHeap() {
		root_list_.clear();
	}
	BinomialHeap(const int &t) {
		Node new_node;
		new_node.key_ = t;
		root_list_.clear();
		root_list_.push_back(new_node);
		min_root_ = -1;
	}
	~BinomialHeap() {
		root_list_.clear();
	}
	BinomialHeap& operator =(const BinomialHeap &from) {
		if (this == &from) {
			return *this;
		}
		root_list_ = from.root_list_;
		min_root_ = find_min();
		return *this;
	}

	void Meld(IHeap *other) { //во второй
		BinomialHeap *second = dynamic_cast <BinomialHeap*>(other);
		BinomialHeap heap;
		if (root_list_.size() == 0) {
			*this = *second;
			return;
		}
		if (second->root_list_.size() == 0) {
			return;
		}
		//предполагаем, что в векторе по неубыванию степени
		std::merge(root_list_.begin(), root_list_.end(),
			second->root_list_.begin(), second->root_list_.end(),
			std::back_inserter(heap.root_list_));

		root_list_.clear();

		for (int i = 0; i < heap.root_list_.size(); i++) {
			if (i + 1 == heap.root_list_.size() || heap.root_list_[i].deg_ != heap.root_list_[i + 1].deg_ || (i + 2 < heap.root_list_.size() &&
				heap.root_list_[i + 1].deg_ == heap.root_list_[i].deg_ && heap.root_list_[i + 1].deg_ == heap.root_list_[i + 2].deg_)) {
				root_list_.push_back(heap.root_list_[i]);
			}
			else {
				if (heap.root_list_[i].key_ < heap.root_list_[i + 1].key_)
					std::swap(heap.root_list_[i], heap.root_list_[i + 1]);

				heap.root_list_[i + 1].deg_++;
				Node *oldChild = heap.root_list_[i + 1].child_;
				if (&heap.root_list_[i])
					heap.root_list_[i + 1].child_ = new Node(heap.root_list_[i]);
				else
					heap.root_list_[i + 1].child_ = nullptr;

				heap.root_list_[i + 1].child_->right_ = oldChild;
			}
		}
		/*std::reverse(heap.root_list_.begin(), heap.root_list_.end());
		for (int i = heap.root_list_.size() - 1; i > 0; i--) {
			//"—хлопываем" равные степени с последнего
			if (heap.root_list_[i].deg_ == heap.root_list_[i - 1].deg_) {
				if (i > 1 && heap.root_list_[i - 1].deg_ == heap.root_list_[i - 2].deg_) {//3 подр€д
					i--;
				}
				if (heap.root_list_[i].key_ < heap.root_list_[i - 1].key_)
					std::swap(heap.root_list_[i], heap.root_list_[i - 1]);

				heap.root_list_[i - 1].deg_++;
				Node *oldChild = heap.root_list_[i - 1].child_;
				if (&heap.root_list_[i])
					heap.root_list_[i - 1].child_ = new Node(heap.root_list_[i]);
				else
					heap.root_list_[i - 1].child_ = nullptr;
				heap.root_list_[i - 1].child_->right_ = oldChild;

				root_list_.push_back(heap.root_list_[i - 1]);

				heap.root_list_.erase(heap.root_list_.begin() + i);
			}
		}
		std::reverse(heap.root_list_.begin(), heap.root_list_.end());
		*/

		min_root_ = find_min();
	}
	int ExtractMin() {
		BinomialHeap inside;

		Node *cur = root_list_[min_root_].child_;
		while (cur != nullptr) {
			inside.root_list_.push_back(*cur);
			cur = cur->right_;
		}
		std::sort(inside.root_list_.begin(), inside.root_list_.end());
		int min_val = root_list_[min_root_].key_;
		root_list_.erase(root_list_.begin() + min_root_);

		Meld(&inside);

		return min_val;
	}
	int GetMin() {
		return root_list_[min_root_].key_;
	}

	void Add(const int &t) {
		Node new_node;
		new_node.key_ = t;

		BinomialHeap new_heap;
		new_heap.root_list_.push_back(new_node);
		new_heap.min_root_ = 0;

		Meld(&new_heap);
	}

private:
	class Node {
	public:
		Node() {
			child_ = nullptr;
			right_ = nullptr;
			deg_ = 0;
		}
		~Node() {
		}
		Node& operator =(const Node &from) {
			if (this == &from)
				return *this;
			key_ = from.key_;
			child_ = from.child_;
			right_ = from.right_;
			deg_ = from.deg_;

			return *this;
		}

		bool operator <(const Node &other) const {
			return deg_ < other.deg_;
		}
	private:
		friend class BinomialHeap;
		int key_;
		Node *child_;				//the most left of ALL childs
		Node *right_;				//next in list of childs (with one parent_)
		int deg_;					//num of ALL childs
	};

	int find_min() {
		int mini = INT_MAX;
		int index = 0;
		for (size_t i = 0; i < root_list_.size(); i++) {
			if (root_list_[i].key_ < mini) {
				mini = root_list_[i].key_;
				index = i;
			}
		}
		min_root_ = index;
		return index;
	}

	int min_root_;
	std::vector<Node> root_list_;
};