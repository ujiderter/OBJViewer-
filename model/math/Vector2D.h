#ifndef OBJVIEWER_VECTOR2D_H
#define OBJVIEWER_VECTOR2D_H

#include <cmath>
#include <concepts>
#include <iostream>
#include <type_traits>


#include "VecMathCommon.h"

namespace VecMath {

    template<Numeric T>
    class Vector2D {
    public:
        T x, y;

        constexpr Vector2D() noexcept: x(0), y(0) {}

        constexpr Vector2D(T x_, T y_) noexcept: x(x_), y(y_) {}

        constexpr Vector2D &operator+=(const Vector2D &other) noexcept {
            x += other.x;
            y += other.y;
            return *this;
        }

        constexpr Vector2D &operator-=(const Vector2D &other) noexcept {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        constexpr Vector2D &operator*=(T scalar) noexcept {
            x *= scalar;
            y *= scalar;
            return *this;
        }

        [[nodiscard]] constexpr T lengthSquared() const noexcept {
            return x * x + y * y;
        }

        [[nodiscard]] double length() const noexcept {
            return std::sqrt(static_cast<double>(lengthSquared()));
        }

        [[nodiscard]] Vector2D normalized() const {
            double len = length();
            if (len == 0) return Vector2D{};
            return Vector2D{static_cast<T>(x / len), static_cast<T>(y / len)};
        }

        [[nodiscard]] constexpr T dot(const Vector2D &other) const noexcept {
            return x * other.x + y * other.y;
        }
    };

    // Бинарные операторы
    template<Numeric T>
    [[nodiscard]] constexpr Vector2D<T> operator+(Vector2D<T> lhs, const Vector2D<T> &rhs) noexcept {
        return lhs += rhs;
    }

    template<Numeric T>
    [[nodiscard]] constexpr Vector2D<T> operator-(Vector2D<T> lhs, const Vector2D<T> &rhs) noexcept {
        return lhs -= rhs;
    }

    template<Numeric T>
    [[nodiscard]] constexpr Vector2D<T> operator*(Vector2D<T> vec, T scalar) noexcept {
        return vec *= scalar;
    }

    template<Numeric T>
    [[nodiscard]] constexpr Vector2D<T> operator*(T scalar, Vector2D<T> vec) noexcept {
        return vec *= scalar;
    }

    template<Numeric T>
    std::ostream &operator<<(std::ostream &os, const Vector2D<T> &vec) {
        return os << "(" << vec.x << ", " << vec.y << ")";
    }
}

#endif //OBJVIEWER_VECTOR2D_H
