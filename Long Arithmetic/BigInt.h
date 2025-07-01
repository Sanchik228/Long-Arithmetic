#ifndef BIGINT
#define BIGINT

#include <iostream>
#include <vector>
#include <string>

class BigInt {
private:
	std::vector<int> digits;
	bool is_negative = false;

	BigInt add_abs(const BigInt& other) const;
	BigInt sub_abs(const BigInt& other) const;
	int abs_compare(const BigInt& other) const;
public:
	BigInt();
	BigInt(int value);
	BigInt(const std::string& s);

	std::string to_string() const;
	bool operator==(const BigInt& other) const;
	bool operator!=(const BigInt& other) const;
	bool operator<(const BigInt& other) const;
	bool operator<=(const BigInt& other) const;
	bool operator>(const BigInt& other) const;
	bool operator>=(const BigInt& other) const;
	BigInt operator+(const BigInt& other) const;
	BigInt operator-(const BigInt& other) const;
	BigInt operator*(const BigInt& other) const;
	BigInt operator/(const BigInt& other) const;
	BigInt operator%(const BigInt& other) const;
	BigInt operator^(const BigInt& exponent) const {
		return pow(exponent);
	}
	unsigned long long to_ull() const;
	BigInt pow(const BigInt& exponent) const;
	int to_int() const;
	long long to_ll() const;
	friend std::ostream& operator<<(std::ostream& os, const BigInt& n);
};

#endif