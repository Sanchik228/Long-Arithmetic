#include "BigInt.h"
#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <limits>

BigInt::BigInt() : digits(1, 0), is_negative(false) {}

BigInt::BigInt(int value) {
	if (value < 0) {
		is_negative = true;
		value = -value;
	}
	else {
		is_negative = false;
	}
	if (value == 0) {
		digits.push_back(0);
	}
	else {
		while (value > 0) {
			digits.push_back(value % 10);
			value /= 10;
		}
	}
}

BigInt::BigInt(const std::string& s) {
	is_negative = false;
	digits.clear();

	size_t start = 0;
	if (s.empty()) throw std::invalid_argument("Empty string");
	if (s[0] == '-') {
		is_negative = true;
		start = 1;
	}
	else if (s[0] == '+') {
		start = 1;
	}

	for (size_t i = s.size(); i > start; i--) {
		char c = s [i - 1];
		if (!std::isdigit(c)) throw std::invalid_argument("Invalid character in number");
		digits.push_back(c - '0');
	}
	while (digits.size() > 1 && digits.back() == 0) {
		digits.pop_back();
	}
	if (digits.size() == 1 && digits[0] == 0) {
		is_negative = false;
	}
}

std::string BigInt::to_string() const {
	std::string s;
	if (is_negative) s += '-';
	for (auto i = digits.rbegin(); i != digits.rend(); i++) {
		s += ('0' + *i);
	}
	return s;
}

BigInt BigInt::add_abs(const BigInt& other) const {
	BigInt result;
	result.digits.clear();

	const size_t n = std::max(digits.size(), other.digits.size());
	int carry = 0;
	for (size_t i = 0; i < n || carry != 0; i++) {
		int digit_sum = carry;
		if (i < digits.size()) digit_sum += digits[i];
		if (i < other.digits.size()) digit_sum += other.digits[i];
		result.digits.push_back(digit_sum % 10);
		carry = digit_sum / 10;
	}
	return result;
}

int BigInt::abs_compare(const BigInt& other) const {
	if (digits.size() != other.digits.size())
		return digits.size() < other.digits.size() ? -1 : 1;
	for (int i = (int)digits.size() - 1; i >= 0; --i) {
		if (digits[i] != other.digits[i])
			return digits[i] < other.digits[i] ? -1 : 1;
	}
	return 0;
}

BigInt BigInt::sub_abs(const BigInt& other) const {
	BigInt result;
	result.digits.clear();

	int borrow = 0;
	for (size_t i = 0; i < digits.size(); ++i) {
		int digit_sub = digits[i] - borrow;
		if (i < other.digits.size()) {
			digit_sub -= other.digits[i];
		}
		if (digit_sub < 0) {
			digit_sub += 10;
			borrow = 1;
		}
		else {
			borrow = 0;
		}
		result.digits.push_back(digit_sub);
	}
	while (result.digits.size() > 1 && result.digits.back() == 0) {
		result.digits.pop_back();
	}

	return result;
}

bool BigInt::operator==(const BigInt& other) const {
	return is_negative == other.is_negative && digits == other.digits;
}

bool BigInt::operator!=(const BigInt& other) const {
	return !(*this == other);
}

bool BigInt::operator<(const BigInt& other) const {
	if (is_negative != other.is_negative) return is_negative;

	int cmp = abs_compare(other);
	if (is_negative) return cmp > 0;
	else return cmp < 0;
}

bool BigInt::operator<=(const BigInt& other) const {
	return *this < other || *this == other;
}

bool BigInt::operator>(const BigInt& other) const {
	return !(*this <= other);
}

bool BigInt::operator>=(const BigInt& other) const {
	return !(*this < other);
}

BigInt BigInt::operator+(const BigInt& other) const {
	if (is_negative == other.is_negative) {
		BigInt res = add_abs(other);
		res.is_negative = is_negative;
		return res;
	}
	else {
		int cmp = abs_compare(other);
		if (cmp == 0) return BigInt(0);
		else if (cmp > 0) {
			BigInt res = sub_abs(other);
			res.is_negative = is_negative;
			return res;
		}
		else {
			BigInt res = other.sub_abs(*this);
			res.is_negative = other.is_negative;
			return res;
		}
	}
}

BigInt BigInt::operator-(const BigInt & other) const {
	if (is_negative == other.is_negative) {
		int cmp = abs_compare(other);
		if (cmp == 0) {
			return BigInt(0);
		}
		if (cmp > 0) {
			BigInt result = sub_abs(other);
			result.is_negative = is_negative;
			return result;
		}
		else {
			BigInt result = other.sub_abs(*this);
			result.is_negative = !is_negative;
			return result;
		}
	}
	else {
		BigInt result = add_abs(other);
		result.is_negative = is_negative;
		return result;
	}
}

BigInt BigInt::operator*(const BigInt& other) const {
	BigInt result;
	result.digits.assign(digits.size() + other.digits.size(), 0);

	for (size_t i = 0; i < digits.size(); ++i) {
		int carry = 0;
		for (size_t j = 0; j < other.digits.size() || carry != 0; ++j) {
			int64_t cur = result.digits[i + j] +
				digits[i] * 1LL * (j < other.digits.size() ? other.digits[j] : 0) +
				carry;
			result.digits[i + j] = cur % 10;
			carry = cur / 10;
		}
	}
	while (result.digits.size() > 1 && result.digits.back() == 0)
		result.digits.pop_back();
	result.is_negative = (is_negative != other.is_negative) 
		&& !(result.digits.size() == 1 && result.digits[0] == 0);

	return result;
}

BigInt BigInt::operator/(const BigInt& other) const {
	if (other == BigInt(0)) {
		throw std::runtime_error("Division by zero");
	}

	BigInt dividend = *this;
	BigInt divisor = other;

	dividend.is_negative = false;
	divisor.is_negative = false;

	BigInt result;
	result.digits.resize(dividend.digits.size(), 0);

	BigInt current;
	for (int i = (int)dividend.digits.size() - 1; i >= 0; --i) {
		current.digits.insert(current.digits.begin(), dividend.digits[i]);
		while (current.digits.size() > 1 && current.digits.back() == 0) {
			current.digits.pop_back();
		}

		int x = 0, l = 0, r = 9;
		while (l <= r) {
			int m = (l + r) / 2;
			BigInt t = divisor * BigInt(m);
			if (t <= current) {
				x = m;
				l = m + 1;
			}
			else {
				r = m - 1;
			}
		}
		result.digits[i] = x;
		current = current - (divisor * BigInt(x));
	}
	while (result.digits.size() > 1 && result.digits.back() == 0) result.digits.pop_back();

	result.is_negative = (is_negative != other.is_negative) 
		&& !(result.digits.size() == 1 
			&& result.digits[0] == 0);

	return result;
}

BigInt BigInt::operator%(const BigInt& other) const {
	return *this - (*this / other) * other;
}

unsigned long long BigInt::to_ull() const {
	unsigned long long result = 0;
	unsigned long long base = 1;
	for (size_t i = 0; i < digits.size(); ++i) {
		if (digits[i] != 0 && base > ULLONG_MAX / 10) {
			throw std::overflow_error("BigInt too big for unsigned long long");
		}
		result += digits[i] * base;
		base *= 10;
		if (result > ULLONG_MAX) {
			throw std::overflow_error("BigInt too big for unsigned long long");
		}
	}
	if (is_negative && result != 0) {
		throw std::invalid_argument("Negative BigInt cannot be converted to unsigned long long");
	}
	return result;
}

BigInt BigInt::pow(const BigInt& exponent) const {
	if (exponent.is_negative) {
		throw std::invalid_argument("Negative exponent not supported in BigInt pow");
	}
	unsigned long long exp = exponent.to_ull();

	BigInt base = *this;
	BigInt result(1);
	while (exp > 0) {
		if (exp % 2 == 1) {
			result = result * base;
		}
		base = base * base;
		exp /= 2;
	}
	return result;
}

int BigInt::to_int() const {
	unsigned long long ull = to_ull();
	if (ull > static_cast<unsigned long long>(std::numeric_limits<int>::max())) {
		throw std::overflow_error("BigInt too big for int");
	}
	if (is_negative) {
		if (ull > static_cast<unsigned long long>(std::numeric_limits<int>::max()) + 1ULL) {
			throw std::overflow_error("BigInt too small for int");
		}
		return -static_cast<int>(ull);
	}
	return static_cast<int>(ull);
}

long long BigInt::to_ll() const {
	unsigned long long ull = to_ull();
	if (ull > static_cast<unsigned long long>(std::numeric_limits<long long>::max())) {
		throw std::overflow_error("BigInt too big for long long");
	}
	if (is_negative) {
		if (ull > static_cast<unsigned long long>(std::numeric_limits<long long>::max()) + 1ULL) {
			throw std::overflow_error("BigInt too small for long long");
		}
		return -static_cast<long long>(ull);
	}
	return static_cast<long long>(ull);
}

std::ostream& operator<<(std::ostream& s, const BigInt& n) {
	s << n.to_string();
	return s;
}