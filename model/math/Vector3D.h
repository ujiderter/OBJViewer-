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

        // Конструкторы
        constexpr Vector3D() noexcept : x(0), y(0), z(0) {}

        constexpr Vector3D(T x_, T y_, T z_) noexcept : x(x_), y(y_), z(z_) {}

        // Оператор +=
        constexpr Vector3D &operator+=(const Vector3D &other) noexcept {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        // Оператор -=
        constexpr Vector3D &operator-=(const Vector3D &other) noexcept {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }

        // Оператор *= (умножение на скаляр)
        constexpr Vector3D &operator*=(T scalar) noexcept {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }

        // Оператор /=
        constexpr Vector3D &operator/=(T scalar) {
            if (scalar == 0) {
                throw std::runtime_error("Division by zero in Vector3D");
            }
            x /= scalar;
            y /= scalar;
            z /= scalar;
            return *this;
        }

        // Длина вектора (квадрат длины)
        [[nodiscard]] constexpr T lengthSquared() const noexcept {
            return x * x + y * y + z * z;
        }

        // Длина вектора
        [[nodiscard]] double length() const noexcept {
            return std::sqrt(static_cast<double>(lengthSquared()));
        }

        // Нормализация вектора
        [[nodiscard]] Vector3D normalized() const {
            double len = length();
            if (len == 0) return Vector3D{}; // Возвращаем нулевой вектор, если длина равна нулю
            return Vector3D{static_cast<T>(x / len), static_cast<T>(y / len), static_cast<T>(z / len)};
        }

        // Скалярное произведение
        [[nodiscard]] constexpr T dot(const Vector3D &other) const noexcept {
            return x * other.x + y * other.y + z * other.z;
        }

        // Векторное произведение
        [[nodiscard]] constexpr Vector3D cross(const Vector3D &other) const noexcept {
            return Vector3D{
                    y * other.z - z * other.y,
                    z * other.x - x * other.z,
                    x * other.y - y * other.x
            };
        }

        // Проверка на равенство
        [[nodiscard]] constexpr bool operator==(const Vector3D &other) const noexcept {
            return x == other.x && y == other.y && z == other.z;
        }

        // Проверка на неравенство
        [[nodiscard]] constexpr bool operator!=(const Vector3D &other) const noexcept {
            return !(*this == other);
        }

        constexpr void normalize() {
            double len = length();
            if (len == 0) {
                throw std::runtime_error("Cannot normalize a zero-length vector");
            }
            x = static_cast<T>(x / len);
            y = static_cast<T>(y / len);
            z = static_cast<T>(z / len);
        }
    };

    // Оператор +
    template<Numeric T>
    [[nodiscard]] constexpr Vector3D<T> operator+(Vector3D<T> lhs, const Vector3D<T> &rhs) noexcept {
        return lhs += rhs;
    }

    // Оператор -
    template<Numeric T>
    [[nodiscard]] constexpr Vector3D<T> operator-(Vector3D<T> lhs, const Vector3D<T> &rhs) noexcept {
        return lhs -= rhs;
    }

    // Унарный минус
    template<Numeric T>
    [[nodiscard]] constexpr Vector3D<T> operator-(const Vector3D<T> &vec) noexcept {
        return Vector3D<T>{-vec.x, -vec.y, -vec.z};
    }

    // Оператор * (умножение вектора на скаляр)
    template<Numeric T>
    [[nodiscard]] constexpr Vector3D<T> operator*(Vector3D<T> vec, T scalar) noexcept {
        return vec *= scalar;
    }

    // Оператор * (умножение скаляра на вектор)
    template<Numeric T>
    [[nodiscard]] constexpr Vector3D<T> operator*(T scalar, Vector3D<T> vec) noexcept {
        return vec *= scalar;
    }

    // Оператор / (деление вектора на скаляр)
    template<Numeric T>
    [[nodiscard]] constexpr Vector3D<T> operator/(Vector3D<T> vec, T scalar) noexcept {
        return vec /= scalar;
    }

    // Вывод в поток
    template<Numeric T>
    std::ostream &operator<<(std::ostream &os, const Vector3D<T> &vec) {
        return os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    }
}

#endif //OBJVIEWER_VECTOR3D_H
