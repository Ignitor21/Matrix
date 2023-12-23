#pragma once

#include <iostream>
#include <exception>
#include "matrix-exceptions.hxx"

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
        int rows, columns, used = 0;
        T* elems;

    protected:
        MatrixBuf(const MatrixBuf<T>& other) = delete;
        MatrixBuf<T>& operator= (const MatrixBuf<T>& other) = delete;

        MatrixBuf(MatrixBuf<T>&& other) noexcept
         : rows(other.rows), columns(other.columns), used(other.used), elems(other.elems)
        {
            other.rows = 0;
            other.columns = 0;
            other.used = 0;
            other.elems = nullptr;
        }

        MatrixBuf<T>& operator= (MatrixBuf<T>&& other) noexcept
        {
            if (*this == other)
                return *this;
            
            std::swap(elems, other.elems);
            std::swap(columns, other.columns);
            std::swap(used, other.used);
            std::swap(rows, other.rows);
            return *this;
        }

        ~MatrixBuf()
        {
            destroy(elems, elems + used);
            ::operator delete(elems);
        }

        MatrixBuf(int r, int c) : rows(r), columns(c)
        {
            if (r <= 0 || c <= 0)
                throw invalid_size();

            elems = static_cast<T*>(::operator new(sizeof(T) * r * c));
        }
    };

    template <typename T = double>
    class Matrix : private MatrixBuf<T>
    {
        using MatrixBuf<T>::rows;
        using MatrixBuf<T>::columns;
        using MatrixBuf<T>::used;
        using MatrixBuf<T>::elems;

        struct ProxyRow
        {
            T *row;
            int columns;

            const T& operator[](int n) const 
            {
                if ((n < 0) || (n >= columns))
                    throw invalid_index();

                return row[n];
            }

            T& operator[](int n)
            {
                if ((n < 0) || (n >= columns))
                    throw invalid_index();

                return row[n];
            }
        };

    public:
        Matrix(int r, int c) : MatrixBuf<T>(r, c) {}        
        
        Matrix(Matrix&& other) = default;
        Matrix& operator=(Matrix&& other) = default;
        ~Matrix() = default;

        Matrix(const Matrix& other) : MatrixBuf<T>(other.rows, other.columns)
        {
            while (used < other.used)
            {
                construct(elems + used, other.elems[used]);
                ++used;
            }
        }

        Matrix& operator= (const Matrix& other)
        {
            Matrix<T> tmp{other};
            std::swap(*this, tmp);
            return *this;
        }
    
    public:
        int getRows() const noexcept { return rows; }
        int getColumns() const noexcept { return columns; }
        ProxyRow operator[] (int n) const 
        {
            if ((n < 0) || (n >= rows))
                throw invalid_index();
            return ProxyRow{elems + n * columns, columns};
        }
    };

    template <typename T>
    std::ostream& operator<< (std::ostream& os, const Matrix<T>& m) 
    {
        int cols = m.getColumns(), rows = m.getRows();

        for (int i = 0; i != rows; ++i)
        {
            for (int j = 0; j != cols; ++j)
            {
                os << m[i][j] << ' ';
            }

            os << "\n";
        }

        return os;
    }

    template <typename T>
    std::istream& operator>> (std::istream& is, Matrix<T>& m)
    {
        int cols = m.getColumns(), rows = m.getRows();

        for (int i = 0; i != rows; ++i)
        {
            for (int j = 0; j != cols; ++j)
            {
                is >> m[i][j];
            }
        }

        return is;
    }
}
