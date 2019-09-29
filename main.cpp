#include <cstdint>
#include <iostream>
#include "Generator.hpp"

using namespace std;

ostream& print(ostream& out, const uint_fast64_t p) {
    out << "IrreduciblePolynomialQ[";
    for (uint_fast8_t i = 0; i < 64; ++i) {
        if (p & (1ull << i)) { out << "+x^" << +i; }
    }
    out << ", Modulus -> 2]" << endl;
    return out;
}

int main() {
    print(cout, Generator::GetIrrPoly(48));
    return 0;
}