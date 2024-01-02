#pragma once

#include <iostream>
#include <exception>
#include <cmath>
#include <utility>
#include <iomanip>
#include "matrix-exceptions.hxx"

static const double EPSILON = 10E-8;

bool is_equal(double lhs, double rhs)
{
    return (std::fabs(rhs - lhs) < EPSILON);
}

namespace linal
{

    template <typename T> void construct(T *p, const T& rhs) { new (p) T(rhs); }

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
            if (this == &   other)
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

        MatrixBuf(int r, int c) : rows(r), columns(c), used(r*c)
        {
            if (r <= 0 || c <= 0)
                throw invalid_size();

            elems = static_cast<T*>(::operator new(sizeof(T) * r * c));
        }
    };

    template <typename T = double>
    class Matrix final : private MatrixBuf<T>
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
            for(used = 0; used < other.used; ++used)
                construct(elems + used, other.elems[used]);
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

        double determinant() const;

    private:
        double diag_mult() const;
        bool gauss_jordan(int& number_of_swaps);
        int max_abs_elem_in_column(int column_index) const;
        bool swap_rows(int i, int j);
        void eleminate(int i);
    };

    template <typename T>
    double Matrix<T>::determinant() const
    {
        if (rows != columns)
            throw nonsquare_matrix();
        
        Matrix<T> tmp{*this};
        #ifdef DEBUG
            std::cout << tmp << "\n";
        #endif
        int number_of_swaps{};
        bool is_zero = tmp.gauss_jordan(number_of_swaps);

        if (is_zero)
            return 0;

        double ans = tmp.diag_mult();

        if (number_of_swaps % 2 == 1)
            ans *= -1;

        return ans;
    }

    template <typename T>
    double Matrix<T>::diag_mult() const
    {
        double ans = 1;
        for (int i = 0; i < rows * rows; i += rows + 1)
        {
            ans *= elems[i];
        }

        #ifdef DEBUG
            std::cout << "Diag mult: " << ans << "\n";
        #endif
        return ans;
    }

    template <typename T>
    bool Matrix<T>::gauss_jordan(int& number_of_swaps)
    {
        for (int i = 0; i != columns; ++i)
        {
            int max_elem_index = max_abs_elem_in_column(i);

            if (is_equal(elems[max_elem_index * columns + i], 0))
                return true;
            
            number_of_swaps += swap_rows(max_elem_index, i); 
            #ifdef DEBUG
                std::cout << *this << "\n";
            #endif

            eleminate(i);
            #ifdef DEBUG
                std::cout << *this << "\n";
            #endif
        }

        return false;

    }

    template <typename T>
    int Matrix<T>::max_abs_elem_in_column(int column_index) const
    {
        T *cur = elems + column_index * (columns + 1);
        int ans{};
        T max_elem{};

        for(int i = column_index; i < rows; i += 1, cur += columns)
        {
            T cur_elem_abs = std::fabs(*cur);
            if (cur_elem_abs > max_elem)
            {
                max_elem = cur_elem_abs;
                ans = i;
            }
        }
        #ifdef DEBUG
            std::cout << "Max element in column: " << column_index << " is " << max_elem << "\n";
        #endif
        return ans;
    }

    template <typename T>
    bool Matrix<T>::swap_rows(int i, int j)
    {
        if(i == j)
            return false;
        
        T* first_row = elems + i * columns;
        T* second_row = elems + j * columns;

        for (int k = 0; k < columns; ++k)
            std::swap(first_row[k], second_row[k]);
        
        return true;
    }

    template <typename T>
    void Matrix<T>::eleminate(int i)
    {
        for (int j = i + 1; j < rows; ++j)
        {
            T pivot = elems[i * (columns + 1)];
            T k = -1 * elems[ j*columns + i ] / pivot;

            if (k == 0)
                continue;

            for (int m = i; m < columns; ++m)
                elems[j * columns + m] += elems[i * columns + m] * k;
        }
    }

    template <typename T>
    std::ostream& operator<< (std::ostream& os, const Matrix<T>& m) 
    {
        int cols = m.getColumns(), rows = m.getRows();

        for (int i = 0; i != rows; ++i)
        {
            for (int j = 0; j != cols; ++j)
            {
                std::cout << m[i][j] << ' ';
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

    template <typename T>
    bool operator== (const Matrix<T>& lhs, const Matrix<T>& rhs)
    {
        if((lhs.getRows() != rhs.getRows()) || (lhs.getColumns() != lhs.getColumns()))
            return false;

        int rows = lhs.getRows(), columns = lhs.getColumns();

        for (int i = 0; i != rows; ++i)
        {
            for (int j = 0; j != columns; ++j)
            {
                if (lhs[i][j] != rhs[i][j])
                    return false;
            }
        }

        return true;
    }

    template <typename T>
    bool operator!= (const Matrix<T>& lhs, const Matrix<T>& rhs)
    {
        return !(lhs == rhs);
    }
}
