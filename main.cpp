#include <cstdint>
#include <iostream>
#include "Generator.hpp"

using namespace std;

ostream &print(ostream &out, const uint_fast64_t p) {
    out << "IrreduciblePolynomialQ[";
    for (uint_fast8_t i = 0; i < 64; ++i) {
        if (p & (1ull << i)) { out << "+x^" << +i; }
    }
    out << ", Modulus -> 2]" << endl;
    return out;
}

#include <chrono>
#include <fstream>
#include "Polynomial.hpp"
#include "Random.hpp"

uint_fast64_t GenerateAll(const uint_fast8_t degree) {
    uint_fast64_t n = (1ull << degree), res = 0;
    for (uint_fast64_t i = 0; i < n; ++i) {
        res += Polynomial((1ull << degree) | (Random(degree - 1ull) << 1ull) | 1ull)
                .IsIrredusible(degree);
    }
    return res;
}

void WriteTimings(const char file[]) {
    ofstream out(file);
    chrono::steady_clock::time_point start, end;
    uint_fast64_t res;
    for (uint_fast8_t i = 2; i <= 63; ++i) {
        start = chrono::steady_clock::now();
        res = GenerateAll(i);
        end = chrono::steady_clock::now();
        out << "deg: " << +i << endl
            << "num: " << res << endl
            << "mic: "
            << chrono::duration_cast<chrono::microseconds>(end - start).count()
            << endl
            << "mil: "
            << chrono::duration_cast<chrono::milliseconds>(end - start).count()
            << endl
            << "sec: "
            << chrono::duration_cast<chrono::seconds>(end - start).count()
            << endl
            << "min: "
            << chrono::duration_cast<chrono::minutes>(end - start).count()
            << endl
            << "hou: "
            << chrono::duration_cast<chrono::hours>(end - start).count()
            << endl
            << endl;
        out.flush();
    }
    out.close();
}

int main() {
    print(cout, Generator::GetIrrPoly(48));
    WriteTimings("/Users/vadimpiven/Downloads/timings.txt");
    return 0;
}
