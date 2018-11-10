#include <iostream>
#include <cstdio>


class Permutation {
private:
	int *a;
	int size;
	int min(int a, int b) const {
		if (a > b)
			return b;
		else
			return a;
	}
public:
	Permutation(int n) :size(n) {
		a = new int[size];
		for (int i = 0; i < size; i++) {
			a[i] = i;
		}
	}
	Permutation(int n, int *input) :size(n) {
		a = new int[size];
		for (int i = 0; i < size; i++) {
			a[i] = input[i];
		}
	}
	Permutation(const Permutation& p) :size(p.size) {
		a = new int[size];
		for (int i = 0; i < size; i++) {
			a[i] = p.a[i];
		}
	}
	~Permutation() {
		delete[] a;
	}

	Permutation& operator = (const Permutation& p) {
		if (&p == this)
			return (*this);
		delete[] a;
		a = new int[p.size];
		size = p.size;
		for (int i = 0; i < size; i++) {
			a[i] = p.a[i];
		}
		return (*this);
	}
	const int& operator [](int i)const {
		return a[i];
	}
	bool operator ==(const Permutation& p)const {
		if (size != p.size)
			return false;
		int ind = 0;
		while (ind < size) {
			if (a[ind] != p.a[ind])
				return false;
			ind++;
		}
		return true;
	}
	bool operator !=(const Permutation& p)const {
		return !((*this) == p);
	}
	int findEqPrefix(const Permutation& p)const {
		int ind = 0;
		while (ind < min(size, p.size) && a[ind] == p.a[ind])
			ind++;
		return ind;
	}
	bool operator <=(const Permutation& p)const {
		if ((*this) == p)
			return true;
		int ind = findEqPrefix(p);
		if (ind == p.size && ind != size)
			return false;
		if (ind != p.size && ind == size)
			return true;
		if (ind == p.size && ind == size)
			return true;
		return p.a[ind] > a[ind];
	}
	bool operator <(const Permutation& p)const {
		return (*this <= p && *this != p);
	}
	bool operator >=(const Permutation& p)const {
		return (!(*this < p));
	}
	bool operator >(const Permutation& p)const {
		return (*this >= p && *this != p);
	}
	Permutation& operator *=(const Permutation& p) {
		int *buffer;
		buffer = new int[size];

		for (int i = 0; i < size; i++) {
			buffer[i] = a[p[i]];
		}
		for (int i = 0; i < size; i++) {
			a[i] = buffer[i];
		}

		delete[] buffer;

		return *this;
	}
	Permutation operator *(const Permutation& p)const {
		Permutation buffer(*this);
		return (buffer *= p);
	}
	void operator ()(int *arr)const {
		int *buffer;
		buffer = new int[size];

		for (int i = 0; i < size; i++) {
			buffer[a[i]] = arr[i];
		}

		for (int i = 0; i < size; i++) {
			arr[i] = buffer[i];
		}

		delete[] buffer;
	}
	Permutation inverse()const {
		Permutation rp(size);
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if (a[j] == i) {
					rp.a[i] = j;
				}
			}
		}
		return rp;

	}
	Permutation next()const {
		Permutation result(size);
		if (size == 1)
			return result;
		int ind = size - 2;
		while (ind >= 0 && a[ind] >= a[ind + 1])
			ind--;
		if (ind == -1) {
			for (int i = 0; i < size; i++) {
				result.a[i] = i;
			}
		}
		else {
			int l = ind + 1;
			for (int i = ind + 1; i < size; i++) {
				if (a[ind] < a[i]) //a[l] -> a[ind]
					l = i;
				else
					break;
			}

			int tmp = a[l];
			a[l] = a[ind];
			a[ind] = tmp;

			for (int i = 0; i < ind + 1; i++)
				result.a[i] = a[i];
			for (int i = ind + 1; i < size; i++)
				result.a[i] = a[size + ind - i];
		}
		return result;

	}
	Permutation previous()const {
		Permutation result(size);
		if (size == 1)
			return result;

		int ind = size - 2;
		while (ind > 0 && a[ind] <= a[ind + 1])
			ind--;

		if (ind == -1) {
			for (int i = 0; i < size; i++) {
				result.a[i] = size - i - 1;
			}
		}

		else {
			int l = ind + 1;
			if (ind == size - 1)
				l = ind;
			for (int i = ind + 1; i < size; i++) {
				if (a[ind] > a[i])//(a[i] < a[l])
					l = i;
				else
					break;
			}

			int tmp = a[l];
			a[l] = a[ind];
			a[ind] = tmp;

			for (int i = 0; i < ind + 1; i++) {
				result.a[i] = a[i];
			}
			for (int i = ind + 1; i < size; i++) {
				result.a[i] = a[size + ind - i];
			}
		}
		return result;
	}
	Permutation& operator ++() {
		Permutation buffer(this->next());
		for (int i = 0; i < size; i++)
			a[i] = buffer[i];
		return *this;
	}
	Permutation& operator --() {
		Permutation buffer(this->previous());
		for (int i = 0; i < size; i++)
			a[i] = buffer[i];
		return *this;
	}
	Permutation operator ++(int) {
		Permutation tmp(*this);
		operator++();
		return tmp;
	}
	Permutation operator --(int) {
		Permutation tmp(*this);
		operator--();
		return tmp;
	}
	friend std::ostream& operator<<(std::ostream& out, const Permutation& p);
};
