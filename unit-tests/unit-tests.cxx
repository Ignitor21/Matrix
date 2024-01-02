#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <gtest/gtest.h>
#include "matrix.hxx"

using namespace linal;

template <typename T>
void fill_matrix(Matrix<T>& m)
{
    int rows = m.getRows(), cols = m.getColumns();

    for (int i = 0; i != rows; ++i)
    {
        for (int j = 0; j != cols; ++j)
        {
            m[i][j] = i * cols + j;
        }
    }
}

TEST(MATRIX_OPERATIONS, element_access)
{
    Matrix<double> m(2, 4);
    fill_matrix(m);
    EXPECT_EQ(m[0][1], 1);
    EXPECT_THROW(m[10][2], invalid_index);
    EXPECT_THROW(m[1][10], invalid_index);
    EXPECT_THROW(m[-5][2], invalid_index);
    EXPECT_THROW(m[1][-5], invalid_index);
}

TEST(MATRIX_BIG_FIVE, copy_constructor)
{
    Matrix<double> m(5, 6);
    fill_matrix(m);
    Matrix<double> m1{m};
    EXPECT_TRUE(m1 == m);
}

TEST(MATRIX_BIG_FIVE, move_constructor)
{
    Matrix<double> m(5, 6);
    fill_matrix(m);
    Matrix<double> ans{m};
    Matrix<double> m1 = std::move(m);
    EXPECT_TRUE(m1 == ans);
}

TEST(MATRIX_BIG_FIVE, copy_assignment)
{
    Matrix<double> m(5, 6);
    fill_matrix(m);
    Matrix<double> m1(2, 7);
    fill_matrix(m1);
    m1 = m;
    EXPECT_TRUE(m1 == m);
}

TEST(MATRIX_BIG_FIVE, move_assignment)
{
    Matrix<double> m(5, 6);
    fill_matrix(m);
    Matrix<double> ans{m};
    Matrix<double> m1(14, 88);
    m1 = std::move(ans);
    EXPECT_TRUE(m1 == m);
}

int main(int argc, char** argv)
{   
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}