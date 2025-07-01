#include <iostream>
#include "BigInt.h"

int main() {
    BigInt a("12345678901234567890");
    BigInt b(987654321);

    BigInt c = a + b;
    std::cout << "a = " << a << "\n" << "b = " << b << "\n" << "a + b = " << c << "\n";

    return 0;
}