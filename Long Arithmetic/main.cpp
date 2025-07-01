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

int main() {
    BigInt a("12345678901234567890");
    BigInt b(987654321);

    BigInt c = a + b;
    std::cout << "a = " << a << "\n" << "b = " << b << "\n" << "a + b = " << c << "\n";

    BigInt d = a - b;
    std::cout << "a - b = " << d << "\n";

    BigInt g = a * b;
    std::cout << "a * b = " << g << "\n";

    BigInt e(-12345);
    BigInt f("999");

    std::cout << "\ne = " << e << "\nf = " << f << "\ne - f = " << e - f << "\n";
    std::cout << "f - e = " << f - e << "\n";
    std::cout << "e * f = " << e * f << "\n";

    BigInt zero("0");
    std::cout << "\na * 0 = " << a * zero << "\n";
    std::cout << "0 * b = " << b * zero << "\n";

    std::cout << std::endl;
    test_cmp(a, b);
  
    return 0;
}