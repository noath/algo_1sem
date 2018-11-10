#include <iostream>
#include <string>
#include <vector>
#include <string>

class BigInteger {
private:
	std::vector <int> nums;
	int size;
	int sign;
	int max(int a, int b) {
		if (a >= b) {
			return a;
		}
		return b;
	}
	int min(int a, int b) {
		if (a >= b) {
			return b;
		}
		return a;
	}
	BigInteger absolute() const {
		BigInteger res(*this);
		res.sign = 1;
		return res;
	}
public:
	BigInteger() {
		size = 1;
		sign = 1;
		nums.push_back(0);
	}
	BigInteger(const int num) {
		int n = num;
		if (n >= 0)
			sign = 1;
		else {
			sign = -1;
			n = -n;
		}
		size = 0;
		if (n == 0) {
			size = 1;
			nums.push_back(0);
		}
		while (n > 0) {
			nums.push_back(n % 10);
			n /= 10;
			size++;
		}

	}
	BigInteger(const BigInteger& t) {
		size = t.size;
		sign = t.sign;
		for (int i = 0; i < size; i++) {
			nums.push_back(t.nums[i]);
		}
	}
	~BigInteger() {
	}
	bool operator ==(const BigInteger& t) const {
		if (size != t.size) {
			return false;
		}
		if (sign != t.sign) {
			return false;
		}
		for (int i = 0; i < size; i++) {
			if (nums[i] != t.nums[i])
				return false;
		}
		return true;
	}
	BigInteger& operator =(const BigInteger& t) {
		if (this == &t)
			return (*this);
		nums.clear();
		size = t.size;
		sign = t.sign;
		for (int i = 0; i < t.size; i++) {
			nums.push_back(t.nums[i]);
		}
		return (*this);
	}
	bool operator !=(const BigInteger& t) const {
		return (!((*this) == t));
	}
	bool operator >=(const BigInteger& t) const {
		if ((*this) == t)
			return true;

		if (sign > t.sign)
			return true;

		if (sign < t.sign)
			return false;

		if (size > t.size) {
			if (sign > 0)
				return true;
			else
				return false;
		}
		if (size < t.size) {
			if (sign > 0)
				return false;
			else
				return true;
		}
		for (int i = size - 1; i >= 0; i--) {
			if (nums[i] > t.nums[i]) {
				if (sign > 0)
					return true;
				else
					return false;
			}
			if (nums[i] < t.nums[i]) {
				if (sign > 0)
					return false;
				else
					return true;
			}
		}
		return true;
	}
	bool operator >(const BigInteger& t) const {
		return ((*this) >= t && (*this) != t);
	}
	bool operator <(const BigInteger& t) const {
		return (!((*this) >= t));
	}
	bool operator <=(const BigInteger& t) const {
		return ((*this) < t || (*this) == t);
	}
	BigInteger operator -() const {
		BigInteger buffer(*this);
		if (size == 1 && nums[0] == 0)
			return (buffer);
		else {
			buffer.sign *= (-1);
			return (buffer);
		}
	}
	std::string toString() const {
		std::string s = "";
		if (sign < 0) {
			s += "-";
		}
		for (int i = size - 1; i >= 0; i--) {
			char symbol = nums[i] + '0';
			s += symbol;
		}
		return s;
	}
	void shrinkZero() {
		while (size > 0 && nums[size - 1] == 0) {
			nums.pop_back();
			size--;
			if (size == 1 && nums[0] == 0)
				break;
		}
	}
	BigInteger& operator +=(const BigInteger& t) {
		if (sign > t.sign)
			return ((*this) -= (t.absolute()));
		if (sign < t.sign) {
			BigInteger tmp(*this);
			*this = t;
			return ((*this) += tmp);
		}
		int sumSize = BigInteger::max(size, t.size);
		BigInteger sum;
		sum.nums.clear();
		int transfer = 0;

		for (int i = 0; i < sumSize; i++) {
			if (i <= min(size - 1, t.size - 1)) {
				sum.nums.push_back(nums[i] + t.nums[i]);
			}
			else {
				if (size >= t.size) {
					sum.nums.push_back(nums[i]);
				}
				else
					sum.nums.push_back(t.nums[i]);
			}
			if (transfer) {
				sum.nums[i] += transfer;
				transfer = 0;
			}
			transfer = sum.nums[i] / 10;
			sum.nums[i] %= 10;
		}

		if (transfer) {
			sum.nums.push_back(transfer);
			transfer %= 10;
		}

		sum.sign = sign;
		sum.size = sum.nums.size();
		*this = sum;
		shrinkZero();
		return (*this);
	}
	BigInteger& operator -=(const BigInteger& tmp) {
		BigInteger t = tmp;
		if (sign > t.sign)
			return (*this += t.absolute());
		if (sign < t.sign) {
			sign = 1;
			(*this) += t;
			sign = -1;
			return *this;
		}
		BigInteger sub;
		sub.nums.clear();
		if (*this == t) {
			sub.nums.push_back(0);
			sub.size = 1;
			sign = 1;
			this->nums.clear();
			*this = sub;
			return *this;
		}
		if (sign < 0 && t.sign < 0) {
			BigInteger tmp = *this;
			*this = t.absolute();
			*this -= tmp.absolute();
			return *this;
		}

		BigInteger* small;
		BigInteger* big;

		if (*this > t) {
			sub.sign = 1;
			small = &t;
			big = this;
		}
		else {
			sub.sign = -1;
			small = this;
			big = &t;
		}

		for (int i = 0; i < small->size; i++) {
			if (small->nums[i] > big->nums[i]) {
				big->nums[i + 1]--;
				big->nums[i] += 10;
			}
			sub.nums.push_back(big->nums[i] - small->nums[i]);
		}
		if ((int)small->nums.size() > small->size && big->nums[small->size - 1] < small->nums[small->size - 1]) {
			sub.nums.push_back(big->nums[small->size - 1] - small->nums[small->size - 1]);
		}

		if (small->size < big->size) {
			for (int i = small->size; i < big->size - 1; i++) {
				if (i != big->size - 1 && 0 > big->nums[i]) {
					big->nums[i + 1]--;
					big->nums[i] += 10;
				}
				sub.nums.push_back(big->nums[i]);
			}
			sub.nums.push_back(big->nums[big->size - 1]);
		}

		sub.size = sub.nums.size();
		*this = sub;
		shrinkZero();
		return (*this);
	}
	BigInteger& operator *=(const BigInteger &t)
	{

		BigInteger res;
		if (sign == t.sign)
			res.sign = 1;
		else
			res.sign = -1;
		res.size = size + t.size + 2;
		res.nums.clear();
		res.nums = std::vector <int>(res.size, 0);

		for (int i = 0; i < size; i++)
		{
			int r = 0;
			for (int j = 0; j < t.size || r; j++)
			{
				res.nums[i + j] += this->nums[i] * (j < t.size ? t.nums[j] : 0) + r;
				r = res.nums[i + j] / 10;
				res.nums[i + j] -= r * 10;
			}
		}
		int pos = size + t.size;
		while (pos > 0 && !res.nums[pos])
			pos--;
		res.size = pos + 1;
		size = res.size;
		*this = res;
		shrinkZero();
		return *this;
	}
	BigInteger& operator /=(const BigInteger &t)
	{
		BigInteger res;
		if (t.sign == sign)
			res.sign = 1;
		else
			res.sign = -1;
		res.size = size + t.size + 1;
		res.nums.clear();
		res.nums = std::vector <int>(size + t.size + 1, 0);
		BigInteger curVal;
		for (int i = size - 1; i >= 0; i--) {
			curVal = curVal * 10 + nums[i];
			int x = 0;
			int l = 0, r = 10;

			while (l <= r) {

				int m = (l + r) / 2;
				BigInteger cur = t.absolute() * m;
				if (cur <= curVal) {
					x = m;
					l = m + 1;
				}
				else
					r = m - 1;
			}
			res.nums[i] = x;
			curVal = curVal - t.absolute() * x;
		}
		res.shrinkZero();
		*this = res;
		shrinkZero();
		return *this;
	}
	BigInteger operator %=(const BigInteger& t) {
		*this = (*this - ((*this / t) * t));
		return *this;
	}
	BigInteger& operator ++() {
		return (*this += 1);
	}
	BigInteger operator ++(int) {
		BigInteger tmp = *this;
		++*this;
		return tmp;
	}
	BigInteger& operator --() {
		return (*this -= 1);
	}
	BigInteger operator --(int) {
		BigInteger tmp = *this;
		--*this;
		return tmp;
	}
	
	explicit operator bool() const {
		if (*this == 0)
			return false;
		return true;
	}

	friend BigInteger operator *(const BigInteger& t1, const BigInteger& t2);
	friend BigInteger operator -(const BigInteger& t1, const BigInteger& t2);
	friend BigInteger operator +(const BigInteger& t1, const BigInteger& t2);
	friend BigInteger operator /(const BigInteger& t1, const BigInteger& t2);
	friend BigInteger operator %(const BigInteger& t1, const BigInteger& t2);
	friend std::istream& operator>>(std::istream& in, BigInteger& t);
	friend std::ostream& operator<<(std::ostream& out, const BigInteger& t);
};
BigInteger operator +(const BigInteger& t1, const BigInteger& t2) {
	BigInteger buffer(t1);
	buffer += t2;
	return buffer;
}
BigInteger operator -(const BigInteger& t1, const BigInteger& t2) {
	BigInteger buffer(t1);
	buffer -= t2;
	return buffer;
}
BigInteger operator *(const BigInteger& t1, const BigInteger& t2) {
	BigInteger buffer = t1;
	buffer *= t2;
	return buffer;
}
BigInteger operator /(const BigInteger& t1, const BigInteger& t2) {
	BigInteger buffer = t1;
	buffer /= t2;
	return buffer;
}
BigInteger operator %(const BigInteger& t1, const BigInteger& t2) {
	BigInteger tmp = t1;
	tmp %= t2;
	return tmp;
}

std::istream& operator>>(std::istream& in, BigInteger& t) {
	std::string s;
	t.nums.clear();
	in >> s;
	if (s == "0" || s == "-0") {
		t.size = 1;
		t.nums.clear();
		t.nums.push_back(0);
		t.sign = 1;
		return in;
	}
	int isMinus = 0;
	if (s[0] == '-') {
		t.sign = -1;
		isMinus++;
	}
	else {
		t.sign = 1;
	}
	t.size = s.length() - isMinus;
	for (int i = s.length() - 1; i >= isMinus; i--) {
		t.nums.push_back(int(s[i]) - '0');
	}
	return in;
}
std::ostream& operator<<(std::ostream& out, const BigInteger& t) {
	if (t.size == 0) {
		out << 0;
		return out;
	}
	if (t.sign < 0)
		out << "-";
	for (int i = t.size - 1; i >= 0; i--) {
		out << t.nums[i];
	}
	return out;
}
