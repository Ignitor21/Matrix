#include <stdexcept>

namespace linal
{

    struct invalid_size : public std::runtime_error
    {
        invalid_size() : std::runtime_error("Invalid sizes in matrix constructor") {}
    };

    struct invalid_index : public std::out_of_range
    {
        invalid_index() : std::out_of_range("Index out of range") {}
    };

    struct nonsquare_matrix : public std::runtime_error
    {
        nonsquare_matrix() : std::runtime_error("Matrix must be square to call this function") {}
    };
}

