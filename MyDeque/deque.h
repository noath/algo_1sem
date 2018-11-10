#pragma once
#include <iostream>
#include <iterator>
#include <algorithm>
#include <cassert>
#include <memory.h>


template <class Deque_type, class category, class value_type, class difference_type, class pointer, class reference>
class deque_iterator : public std::iterator<category, value_type, difference_type, pointer, reference> {
private:

	difference_type ptr_;
	Deque_type* deque_arr_;

public:

	typedef deque_iterator<Deque_type, category, value_type, difference_type, pointer, reference> iterator;

	deque_iterator(Deque_type *deque, size_t n) {
		ptr_ = static_cast<difference_type>(n);
		deque_arr_ = deque;
	}
	deque_iterator(const iterator& from) {
		ptr_ = from.ptr_;
		deque_arr_ = from.deque_arr_;
	}
	iterator& operator =(const iterator& from) {
		ptr_ = from.ptr_;
		deque_arr_ = from.deque_arr_;
		return *this;
	}
	iterator& operator +=(difference_type value) {
		ptr_ += value;
		return *this;
	}
	iterator& operator -=(difference_type value) {
		ptr_ -= value;
		return *this;
	}
	iterator operator +(difference_type value) const {
		iterator temp(*this);
		return (temp += value);
	}
	iterator operator -(difference_type value) const {
		iterator temp(*this);
		return (temp -= value);
	}
	iterator& operator++() {
		return operator+=(1);
	}
	iterator operator++(int) {
		iterator temp(*this);
		*this += 1;
		return temp;
	}
	iterator& operator--() {
		return operator-=(1);
	}
	iterator operator--(int) {
		iterator temp(*this);
		*this -= 1;
		return temp;
	}
	difference_type operator-(const iterator &it) const {
		difference_type res = ptr_ - it.ptr_;
		return res;
	}
	bool operator==(iterator it) {
		return (ptr_ == it.ptr_ && deque_arr_ == it.deque_arr_);
	}
	bool operator!=(iterator it) {
		return !(*this == it);
	}
	bool operator<(iterator it) {
		return (ptr_ < it.ptr_ && deque_arr_ == it.deque_arr_);
	}
	bool operator>(iterator it) {
		return (ptr_ > it.ptr_ && deque_arr_ == it.deque_arr_);;
	}
	bool operator<=(iterator it) {
		return (*this == it || *this < it);
	}
	bool operator>=(iterator it) {
		return (*this == it || *this > it);
	}
	reference operator*() {
		return (*deque_arr_)[ptr_];
	}
	pointer operator->() {
		return &(*deque_arr_)[ptr_];
	}
	reference operator[](difference_type ind) {
		return *(*this + ind);
	}
};


template <class T>
class Deque {
public:
	typedef deque_iterator<const Deque<T>, std::random_access_iterator_tag, T,
		std::ptrdiff_t, const T*, const T&> const_iterator;

	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

	typedef deque_iterator<Deque<T>, std::random_access_iterator_tag, T,
		std::ptrdiff_t, T*, T&> iterator;

	typedef std::reverse_iterator<iterator> reverse_iterator;

	Deque() {
		arr_size_ = 16;
		deque_size_ = 0;
		head_ = 0; 
		tail_ = 1;
		arr_ = new T[arr_size_];
	}

	Deque(const Deque<T>& from) {
		arr_size_ = from.arr_size_;
		deque_size_ = from.deque_size_;
		head_ = from.head_;
		tail_ = from.tail_;
		arr_ = new T[arr_size_];
		for (size_t i = 0; i < arr_size_; i++) {
			arr_[i] = from.arr_[i];
		}
	}

	~Deque() {
		delete[] arr_;
	}

	void push_back(T new_unit) {

		T *to_new = arr_ + tail_;

		deque_size_++;
		(++tail_) %= arr_size_;

		*to_new = new_unit;

		upgrade();
	}

	void pop_back() {
		assert(!empty());

		(--tail_) = (tail_ + arr_size_) % arr_size_;
		deque_size_--;

		upgrade();

	}

	void push_front(T new_unit) {
		T *to_new = arr_ + head_;

		deque_size_++;
		(--head_) = (head_ + arr_size_) % arr_size_;

		*to_new = new_unit;
		upgrade();
	}

	void pop_front() {
		assert(!empty());

		deque_size_--;
		(++head_) = head_ % arr_size_;

		upgrade();
	}

	T& operator[](size_t ind) {
		return arr_[(head_ + ind + 1) % arr_size_];
	}

	const T& operator[](size_t ind) const {
		return arr_[(head_ + ind + 1) % arr_size_];
	}

	T& front() {
		return (*this)[0];
	}

	T& back() {
		return (*this)[deque_size_ - 1];
	}

	T front() const {
		return (*this)[0];
	}

	T back() const {
		return (*this)[deque_size_ - 1];
	}

	size_t size() const {
		return deque_size_;
	}

	bool empty() const {
		return (deque_size_ == 0);
	}

	iterator begin() {
		return iterator(this, 0);
	}

	iterator end() {
		return iterator(this, deque_size_);
	}

	const_iterator cbegin() const {
		return const_iterator(this, 0);
	}

	const_iterator cend() const {
		return const_iterator(this, deque_size_);
	}

	const_iterator begin() const {
		return cbegin();
	}

	const_iterator end() const {
		return cend();
	}

	reverse_iterator rbegin() {
		return reverse_iterator(end());
	}

	const_reverse_iterator crbegin() const {
		return const_reverse_iterator(cend());
	}

	const_reverse_iterator rbegin() const {
		return crbegin();
	}

	reverse_iterator rend() {
		return reverse_iterator(begin());
	}

	const_reverse_iterator crend() const {
		return const_reverse_iterator(cbegin());
	}

	const_reverse_iterator rend() const {
		return crend();
	}

private:
	size_t arr_size_;
	size_t deque_size_;
	size_t head_; //before first
	size_t tail_; //after last

	T *arr_;
	bool change_size() {
		if (4 * deque_size_ <= arr_size_ && arr_size_ != 16) {
			arr_size_ /= 2;
		}
		else if (arr_size_ == deque_size_) {
			arr_size_ *= 2;
		}
		else
			return false;
		return true;
	}
	void upgrade() {
		size_t old_size = arr_size_;
		if (!change_size())
			return;
		else {
			T *new_deque = new T[arr_size_];
			size_t ind = (head_ + 1) % old_size;
			for (size_t i = 1; i < deque_size_ + 1; i++) {
				new_deque[i % arr_size_] = arr_[ind++];
				ind %= old_size;
			}
			head_ = 0;
			tail_ = deque_size_ + 1;
			delete[] arr_;
			arr_ = new T[arr_size_];
			memcpy(arr_, new_deque, sizeof(T)*arr_size_);
			delete[] new_deque;
		}
	}
};
