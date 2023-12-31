#ifndef GLOBAL_H
#define GLOBAL_H

#include <cmath>

namespace global
{
    template <typename T, typename U>
    inline constexpr bool is_nearly_equal(T a, U b)
    {
        constexpr double epsilon = 1e-6;

        return std::abs(a - b) < epsilon;
    }
}

#endif // GLOBAL_H
