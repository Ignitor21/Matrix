#pragma once

#include <iostream>
#include <exception>

static const double EPSILON = 10E-8;

namespace linal
{

    template <typename T> void construct(T *p, const T &rhs) { new (p) T(rhs); }

    template <class T> void destroy(T *p) noexcept { p->~T(); }

    template <typename FwdIter> void destroy(FwdIter first, FwdIter last) noexcept
    {
        while (first != last)
            destroy(&*first++);
    }

    template <typename T>
    class MatrixBuf
    {
    protected:
        int rows, columns;
        T* elems;

    protected:
        MatrixBuf(const MatrixBuf<T>& other) = delete;
        MatrixBuf<T>& operator= (const MatrixBuf<T>& other) = delete;

        MatrixBuf(MatrixBuf<T>&& other) noexcept : rows(other.rows), columns(other.columns), elems(other.elems)
        {
            other.rows = 0;
            other.columns = 0;
            other.elems = nullptr;
        }

        MatrixBuf<T>& operator= (MatrixBuf<T>&& other) noexcept
        {
            if (*this == other)
                return *this;
            
            std::swap(elems, other.elems);
            std::swap(columns, other.columns);
            std::swap(rows, other.rows);
            return *this;
        }

        ~MatrixBuf()
        {
            destroy(elems, elems + rows * columns);
            ::operator delete(elems);
        }

        MatrixBuf(int r, int c) : rows(r), columns(c), elems( ((r == 0) || (c == 0)) ? nullptr : static_cast<T*>(::operator new(sizeof(T) * r * c))) {}
    };
}

//     template <typename T>
//     std::ostream& operator<< (std::ostream& os, const MatrixBuf<T>& matrix)
//     {)
//         int rows = matrix.getRows(), columns = matrix.getColumns();

//         for (int i = 0; i != rows; ++i)
//         {
//             for (int j = 0; j != columns; ++j)
//             {
//                 os << matrix[i][j] << ' ';
//             }

//             os << '\n';
//         }

//         return os;
//     }

//     template <typename T>
//     std::istream& operator>> (std::istream& is, MatrixBuf<T>& matrix)
//     {   
//         int rows = matrix.getRows(), columns = matrix.getColumns();

//         for (int i = 0; i != rows; ++i)
//         {
//             for (int j = 0; j != columns; ++j)
//             {
//                 is >> matrix[i][j];
//             }
//         }

//         return is;
//     }
// }