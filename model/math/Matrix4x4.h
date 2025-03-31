#ifndef OBJVIEWER__MATRIX4X4_H
#define OBJVIEWER__MATRIX4X4_H

#include "Vector3D.h"

namespace VecMath {
    template<Numeric T>
    class Matrix4x4 {
    public:
        // Элементы матрицы в столбцовом порядке (column-major)
        union {
            T m[16];
            T mm[4][4];
            struct {
                T m00, m10, m20, m30;
                T m01, m11, m21, m31;
                T m02, m12, m22, m32;
                T m03, m13, m23, m33;
            };
        };

        // Конструкторы
        constexpr Matrix4x4() noexcept
                : m00(1), m10(0), m20(0), m30(0),
                  m01(0), m11(1), m21(0), m31(0),
                  m02(0), m12(0), m22(1), m32(0),
                  m03(0), m13(0), m23(0), m33(1) {}

        constexpr Matrix4x4(
                T m00_, T m01_, T m02_, T m03_,
                T m10_, T m11_, T m12_, T m13_,
                T m20_, T m21_, T m22_, T m23_,
                T m30_, T m31_, T m32_, T m33_) noexcept
                : m00(m00_), m10(m10_), m20(m20_), m30(m30_),
                  m01(m01_), m11(m11_), m21(m21_), m31(m31_),
                  m02(m02_), m12(m12_), m22(m22_), m32(m32_),
                  m03(m03_), m13(m13_), m23(m23_), m33(m33_) {}

        // Единичная матрица
        static constexpr Matrix4x4 identity() noexcept {
            return Matrix4x4();
        }

        // Умножение матриц
        constexpr Matrix4x4 operator*(const Matrix4x4& other) const noexcept {
            return Matrix4x4(
                    m00 * other.m00 + m01 * other.m10 + m02 * other.m20 + m03 * other.m30,
                    m00 * other.m01 + m01 * other.m11 + m02 * other.m21 + m03 * other.m31,
                    m00 * other.m02 + m01 * other.m12 + m02 * other.m22 + m03 * other.m32,
                    m00 * other.m03 + m01 * other.m13 + m02 * other.m23 + m03 * other.m33,

                    m10 * other.m00 + m11 * other.m10 + m12 * other.m20 + m13 * other.m30,
                    m10 * other.m01 + m11 * other.m11 + m12 * other.m21 + m13 * other.m31,
                    m10 * other.m02 + m11 * other.m12 + m12 * other.m22 + m13 * other.m32,
                    m10 * other.m03 + m11 * other.m13 + m12 * other.m23 + m13 * other.m33,

                    m20 * other.m00 + m21 * other.m10 + m22 * other.m20 + m23 * other.m30,
                    m20 * other.m01 + m21 * other.m11 + m22 * other.m21 + m23 * other.m31,
                    m20 * other.m02 + m21 * other.m12 + m22 * other.m22 + m23 * other.m32,
                    m20 * other.m03 + m21 * other.m13 + m22 * other.m23 + m23 * other.m33,

                    m30 * other.m00 + m31 * other.m10 + m32 * other.m20 + m33 * other.m30,
                    m30 * other.m01 + m31 * other.m11 + m32 * other.m21 + m33 * other.m31,
                    m30 * other.m02 + m31 * other.m12 + m32 * other.m22 + m33 * other.m32,
                    m30 * other.m03 + m31 * other.m13 + m32 * other.m23 + m33 * other.m33
            );
        }

        // Умножение и присваивание
        constexpr Matrix4x4& operator*=(const Matrix4x4& other) noexcept {
            *this = *this * other;
            return *this;
        }

        // Перспективная проекция
        static Matrix4x4 perspective(float fovY, float aspect, float near, float far) {
            const float tanHalfFov = std::tan(fovY / 2.0f);
            const float range = near - far;

            return Matrix4x4(
                    1.0f / (aspect * tanHalfFov), 0.0f, 0.0f, 0.0f,
                    0.0f, 1.0f / tanHalfFov, 0.0f, 0.0f,
                    0.0f, 0.0f, (near + far) / range, -1.0f,
                    0.0f, 0.0f, (2.0f * near * far) / range, 0.0f
            );
        }

        // Ортографическая проекция
        static Matrix4x4 orthographic(float left, float right, float bottom, float top, float near, float far) {
            return Matrix4x4(
                    2.0f / (right - left), 0.0f, 0.0f, 0.0f,
                    0.0f, 2.0f / (top - bottom), 0.0f, 0.0f,
                    0.0f, 0.0f, -2.0f / (far - near), 0.0f,
                    -(right + left) / (right - left), -(top + bottom) / (top - bottom), -(far + near) / (far - near), 1.0f
            );
        }

        // Матрица вида (look-at)
        static Matrix4x4 lookAt(const Vector3D<T>& eye, const Vector3D<T>& target, const Vector3D<T>& up) {
            const Vector3D<T> z = (eye - target).normalized();
            const Vector3D<T> x = up.cross(z).normalized();
            const Vector3D<T> y = z.cross(x);

            return Matrix4x4(
                    x.x, x.y, x.z, -x.dot(eye),
                    y.x, y.y, y.z, -y.dot(eye),
                    z.x, z.y, z.z, -z.dot(eye),
                    0.0f, 0.0f, 0.0f, 1.0f
            );
        }

        // Трансляция
        static Matrix4x4 translate(const Vector3D<T>& translation) {
            return Matrix4x4(
                    1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, 0.0f,
                    translation.x, translation.y, translation.z, 1.0f
            );
        }

        // Вращение вокруг оси
        static Matrix4x4 rotate(float angle, const Vector3D<T>& axis) {
            const float c = std::cos(angle);
            const float s = std::sin(angle);
            const float t = 1.0f - c;
            const Vector3D<T> n = axis.normalized();

            return Matrix4x4(
                    t * n.x * n.x + c, t * n.x * n.y + s * n.z, t * n.x * n.z - s * n.y, 0.0f,
                    t * n.x * n.y - s * n.z, t * n.y * n.y + c, t * n.y * n.z + s * n.x, 0.0f,
                    t * n.x * n.z + s * n.y, t * n.y * n.z - s * n.x, t * n.z * n.z + c, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f
            );
        }

        // Масштабирование
        static Matrix4x4 scale(const Vector3D<T>& scale) {
            return Matrix4x4(
                    scale.x, 0.0f, 0.0f, 0.0f,
                    0.0f, scale.y, 0.0f, 0.0f,
                    0.0f, 0.0f, scale.z, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f
            );
        }

        // Доступ к элементам
        constexpr T* data() noexcept { return m; }
        constexpr const T* data() const noexcept { return m; }

        // Оператор вывода
        friend std::ostream& operator<<(std::ostream& os, const Matrix4x4& mat) {
            return os << "[" << mat.m00 << ", " << mat.m01 << ", " << mat.m02 << ", " << mat.m03 << "]\n"
                      << "[" << mat.m10 << ", " << mat.m11 << ", " << mat.m12 << ", " << mat.m13 << "]\n"
                      << "[" << mat.m20 << ", " << mat.m21 << ", " << mat.m22 << ", " << mat.m23 << "]\n"
                      << "[" << mat.m30 << ", " << mat.m31 << ", " << mat.m32 << ", " << mat.m33 << "]";
        }
    };
}

#endif //OBJVIEWER__MATRIX4X4_H
