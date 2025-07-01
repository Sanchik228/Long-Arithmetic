#include "BigInt.h"
#include <algorithm>
#include <cctype>
#include <stdexcept>

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

BigInt BigInt::operator+(const BigInt& other) const {
	if (!is_negative && !other.is_negative) return add_abs(other);
	throw std::runtime_error("Addition with negative numbers not implemented yet");
}

BigInt BigInt::operator-(const BigInt & other) const {
	if (!is_negative && !other.is_negative) {
		if (abs_compare(other) >= 0) {
			return sub_abs(other);
		}
		else {
			BigInt result = other.sub_abs(*this);
			result.is_negative = true;
			return result;
		}
	}
	if (is_negative && other.is_negative) {
		return other.sub_abs(*this);
	}
	if (is_negative && !other.is_negative) {
		BigInt result = add_abs(other);
		result.is_negative = true;
		return result;
	}
	if (!is_negative && other.is_negative) {
		return add_abs(other);
	}
	throw std::runtime_error("Unhandled case in operator-");
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

std::ostream& operator<<(std::ostream& s, const BigInt& n) {
	s << n.to_string();
	return s;
}