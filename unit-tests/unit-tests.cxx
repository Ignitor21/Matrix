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

int main(int argc, char** argv)
{   
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}