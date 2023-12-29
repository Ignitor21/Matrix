#include <iostream>
#include "matrix.hxx"

using namespace linal;

int main()
{
    Matrix<double> m(2, 2);
    m[0][0] = 1;
    m[0][1] = 0;
    m[1][0] = 0;
    m[1][1] = 1;
    std::cout << m[0][0] << "\n";
    return 0;
}