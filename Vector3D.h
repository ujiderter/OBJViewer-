#ifndef OBJVIEWER_VECTOR3D_H
#define OBJVIEWER_VECTOR3D_H

#include <cmath>
#include <concepts>
#include <iostream>
#include <type_traits>


#include "VecMathCommon.h"

namespace VecMath {

    template<Numeric T>
    class Vector3D {
    public:
        T x, y, z;

        constexpr Vector3D() noexcept: x(0), y(0), z(0) {}

        constexpr Vector3D(T x_, T y_, T z_) noexcept: x(x_), y(y_), z(z_) {}

        constexpr Vector3D &operator+=(const Vector3D &other) noexcept {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        constexpr Vector3D &operator-=(const Vector3D &other) noexcept {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }

        constexpr Vector3D &operator*=(T scalar) noexcept {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }

        // Длина вектора
        [[nodiscard]] constexpr T lengthSquared() const noexcept {
            return x * x + y * y + z * z;
        }

        [[nodiscard]] double length() const noexcept {
            return std::sqrt(static_cast<double>(lengthSquared()));
        }

        [[nodiscard]] Vector3D normalized() const {
            double len = length();
            if (len == 0) return Vector3D{};
            return Vector3D{static_cast<T>(x / len), static_cast<T>(y / len), static_cast<T>(z / len)};
        }

        [[nodiscard]] constexpr T dot(const Vector3D &other) const noexcept {
            return x * other.x + y * other.y + z * other.z;
        }

        [[nodiscard]] constexpr Vector3D cross(const Vector3D &other) const noexcept {
            return Vector3D{
                    y * other.z - z * other.y,
                    z * other.x - x * other.z,
                    x * other.y - y * other.x
            };
        }
    };

    template<Numeric T>
    [[nodiscard]] constexpr Vector3D<T> operator+(Vector3D<T> lhs, const Vector3D<T> &rhs) noexcept {
        return lhs += rhs;
    }

    template<Numeric T>
    [[nodiscard]] constexpr Vector3D<T> operator-(Vector3D<T> lhs, const Vector3D<T> &rhs) noexcept {
        return lhs -= rhs;
    }

    template<Numeric T>
    [[nodiscard]] constexpr Vector3D<T> operator*(Vector3D<T> vec, T scalar) noexcept {
        return vec *= scalar;
    }

    template<Numeric T>
    [[nodiscard]] constexpr Vector3D<T> operator*(T scalar, Vector3D<T> vec) noexcept {
        return vec *= scalar;
    }

    template<Numeric T>
    std::ostream &operator<<(std::ostream &os, const Vector3D<T> &vec) {
        return os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    }
}

#endif //OBJVIEWER_VECTOR3D_H
