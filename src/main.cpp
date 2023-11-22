#include <iostream>
#include "matrix.hpp"

using namespace linal;

int main()
{
    Matrix<int> m(10, 20);
    m[0][0] = 1;
    m[0][1] = 0;
    m[1][0] = 0;
    m[1][1] = 1;
    std::cout << m;
    return 0;
}