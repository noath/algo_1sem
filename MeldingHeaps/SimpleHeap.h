#pragma once
#include <iostream>
#include <algorithm>
#include <queue>
#include <functional>
#include <vector>
#include "IHeap.h"

class SimpleHeap : public IHeap {
public:
	SimpleHeap() {}
	SimpleHeap(const int &t) {
		heap_.push(t);
	}
	~SimpleHeap() {
	}
	bool empty() {
		return heap_.empty();
	}
	void Meld(IHeap *other) {
		SimpleHeap *second = dynamic_cast <SimpleHeap*>(other);
		while (!second->empty()) {
			heap_.push(second->heap_.top());
			second->heap_.pop();
		}
	}
	void Add(const int &t) {
		heap_.push(t);
	}
	int GetMin() {
		return heap_.top();
	}
	int ExtractMin() {
		int min_val = heap_.top();
		heap_.pop();
		return min_val;
	}
private:
	std::priority_queue<int, std::vector<int>, std::greater<int>> heap_;
};