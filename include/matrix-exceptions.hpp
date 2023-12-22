#include <stdexcept>

namespace linal
{

    struct invalid_size : public std::runtime_error
    {
        invalid_size() : std::runtime_error("Invalid sizes in matrix constructor") {}
    };

}

