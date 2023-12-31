#include <iostream>
#include "matrix.hxx"

using namespace linal;

int main()
{
    int n;
    std::cin >> n;
    Matrix<double> matrix(n, n);
    std::cin >> matrix;
    std:: cout << matrix.determinant() << "\n";
    return 0;
}