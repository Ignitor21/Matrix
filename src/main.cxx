#include <iostream>
#include <iomanip>
#include "matrix.hxx"

using namespace linal;

int main()
{
    int n;
    std::cin >> n;
    Matrix<double> matrix(n, n);
    std::cin >> matrix;
    std::cout << std::setprecision(10) << matrix.determinant() << "\n";
    return 0;
}