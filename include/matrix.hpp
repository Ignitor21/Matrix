#pragma once

#include <iostream>
#include <vector>

static const double EPSILON = 10E-8;

namespace linal
{
    template <typename T = double>
    class Matrix final
    {
    private:
        T* elems;
        int rows, columns;

        struct ProxyRow
        {
            T* row;
            const T& operator[](int n) const { return row[n]; }
            T& operator[](int n) { return row[n]; }
        };
    public:
        Matrix(int r, int c) : elems(new T[r*c]{}), rows(r), columns(c) {}

        ~Matrix() 
        {
            delete[] elems;
        }

        ProxyRow operator[] (int n) {return ProxyRow{elems + n*columns};}

        std::ostream& dump(std::ostream& os) const
        {
            for (int i = 0; i != rows; ++i)
            {
                for (int j = 0; j != columns; ++j)
                {
                    os << *(elems + i*columns + j) << ' ';
                }

                os << '\n';
            }

            return os;
        }
    };

    template <typename T>
    std::ostream& operator<< (std::ostream& os, const Matrix<T>& matrix)
    {
        return matrix.dump(os);
    }
}