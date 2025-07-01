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

BigInt BigInt::operator+(const BigInt& other) const {
	if (!is_negative && !other.is_negative) return add_abs(other);
	throw std::runtime_error("Addition with negative numbers not implemented yet");
}

std::ostream& operator<<(std::ostream& s, const BigInt& n) {
	s << n.to_string();
	return s;
}