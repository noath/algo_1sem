#pragma once
#include <iostream>
#include <algorithm>

class IHeap {
public:
	IHeap() {}
	
	virtual void Meld(IHeap *other) = 0;
	virtual void Add(const int &t) = 0;
	virtual int GetMin() = 0;
	virtual int ExtractMin() = 0;
	virtual ~IHeap() {}
private:
	//empty
};