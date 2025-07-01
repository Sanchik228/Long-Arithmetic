#include <iostream>
#include "BigInt.h"

void test_cmp(const BigInt& a, const BigInt& b) {
    std::cout << "a" << " == " << "b" << " : " << (a == b) << "\n";
    std::cout << "a" << " != " << "b" << " : " << (a != b) << "\n";
    std::cout << "a" << " <  " << "b" << " : " << (a < b) << "\n";
    std::cout << "a" << " <= " << "b" << " : " << (a <= b) << "\n";
    std::cout << "a" << " >  " << "b" << " : " << (a > b) << "\n";
    std::cout << "a" << " >= " << "b" << " : " << (a >= b) << "\n\n";
}

void test_conversions() {
    std::cout << "--- Testing conversions ---\n";

    int i = -123456;
    BigInt big_from_int(i);
    std::cout << "BigInt from int " << i << " = " << big_from_int << "\n";

    std::string str = "-9876543210123456789";
    BigInt big_from_str(str);
    std::cout << "BigInt from string \"" << str << "\" = " << big_from_str << "\n";

    // BigInt -> int (with try/catch)
    try {
        int i_conv = big_from_int.to_int();
        std::cout << "BigInt " << big_from_int << " to int: " << i_conv << "\n";
    }
    catch (const std::exception& e) {
        std::cout << "Exception converting BigInt to int: " << e.what() << "\n";
    }

    try {
        int i_conv2 = big_from_str.to_int();
        std::cout << "BigInt " << big_from_str << " to int: " << i_conv2 << "\n";
    }
    catch (const std::exception& e) {
        std::cout << "Exception converting BigInt to int: " << e.what() << "\n";
    }

    // BigInt -> long long (with try/catch)
    try {
        long long ll_conv = big_from_str.to_ll();
        std::cout << "BigInt " << big_from_str << " to long long: " << ll_conv << "\n";
    }
    catch (const std::exception& e) {
        std::cout << "Exception converting BigInt to long long: " << e.what() << "\n";
    }

    // BigInt -> string
    std::string str_conv = big_from_str.to_string();
    std::cout << "BigInt to string: " << str_conv << "\n";

    std::cout << "--- End of conversions tests ---\n\n";
}

int main() {
    BigInt a("12345678901234567890");
    BigInt b(987654321);

    BigInt c = a + b;
    std::cout << "a = " << a << "\n" << "b = " << b << "\n" << "a + b = " << c << "\n";

    BigInt d = a - b;
    std::cout << "a - b = " << d << "\n";

    BigInt g = a * b;
    std::cout << "a * b = " << g << "\n";

    BigInt q = a / b;
    BigInt r = a % b;
    std::cout << "a / b = " << q << "\n";
    std::cout << "a % b = " << r << "\n";

    BigInt h = 2;
    BigInt power = a ^ h;
    std::cout << "a ^ 2 = " << power << "\n";

    BigInt e(-12345);
    BigInt f("999");
    std::cout << "\ne = " << e << "\nf = " << f << "\ne - f = " << e - f << "\n";
    std::cout << "f - e = " << f - e << "\n";
    std::cout << "e * f = " << e * f << "\n";
    std::cout << "e / f = " << e / f << "\n";
    std::cout << "e % f = " << e % f << "\n";

    BigInt zero("0");
    std::cout << "\na * 0 = " << a * zero << "\n";
    std::cout << "0 * b = " << b * zero << "\n";

    std::cout << std::endl;
    test_cmp(a, b);

    test_conversions();
  
    return 0;
}