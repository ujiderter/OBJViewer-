#ifndef OBJVIEWER_VECMATHCOMMON_H
#define OBJVIEWER_VECMATHCOMMON_H

#include <cmath>
#include <concepts>
#include <iostream>
#include <type_traits>

namespace VecMath {

    // Концепт для числовых типов
    template<typename T>
    concept Numeric = std::is_arithmetic_v<T>;

} // namespace VecMath

#endif //OBJVIEWER_VECMATHCOMMON_H
