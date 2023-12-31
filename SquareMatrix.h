#ifndef SQUARE_MATRIX_H
#define SQUARE_MATRIX_H

#include "global.h"

#include <array>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <random>
#include <set>

template <typename T, std::size_t N>
class SquareMatrix
{
public:
    SquareMatrix() = default;
    ~SquareMatrix() = default;

public:
    // Единичная матрица
    static SquareMatrix<T, N> identity_matrix();

    // Обратная матрица
    static SquareMatrix<T, N> inverse_matrix(const SquareMatrix<T, N>& matrix);

public:
    // Заполнить матрицу случайными числами от min до max включительно
    void randomize(int min, int max);

    // Вывести матрицу в консоль
    void print();

public:
    std::array<T, N>& operator[](int i) { return m_matrix[i]; }
    const std::array<T, N>& operator[](int i) const { return m_matrix[i]; }

    SquareMatrix<T, N> operator*(const SquareMatrix<T, N>& matrix_2);

private:
    // Двумерный массив,
    // представляющий квадратную матрицу,
    // изначально все элементы равны нулю
    std::array<std::array<T, N>, N> m_matrix {};
};

template<typename T, std::size_t N>
SquareMatrix<T, N> SquareMatrix<T, N>::identity_matrix()
{
    SquareMatrix<T, N> result_matrix;

    for (std::size_t i = 0; i < N; ++i)
    {
        result_matrix[i][i] = 1;
    }

    return result_matrix;
}

template<typename T, std::size_t N>
SquareMatrix<T, N> SquareMatrix<T, N>::inverse_matrix
(
    const SquareMatrix<T, N>& matrix
)
{
    auto temp_matrix = matrix;

    auto result_matrix = SquareMatrix<T, N>::identity_matrix();

    // Множество обработанных столбцов,
    // в которых на главной диагонали стоит единица,
    // на остальных местах - ноль
    std::set<std::size_t> processed_columns;

    // Совершать элементарные преобразования,
    // пока все столбцы не будут обработаны
    while (processed_columns.size() != N)
    {
        for (std::size_t k = 0; k < N; ++k)
        {
            // Если столбец уже обработан - перейти к следующему
            if (processed_columns.find(k) != processed_columns.end())
            {
                continue;
            }

            // Если элемент главной диагонали равен нулю -
            // перейти к следующему столбцу
            if (global::is_nearly_equal(temp_matrix[k][k], 0))
            {
                continue;
            }

            // Если элемент главной диагонали не равен единице -
            // поделить всю строку на него
            if (!global::is_nearly_equal(temp_matrix[k][k], 1))
            {
                double divisor = temp_matrix[k][k];
                for (std::size_t j = 0; j < N; ++j)
                {
                    temp_matrix[k][j] /= divisor;
                    result_matrix[k][j] /= divisor;
                }
            }

            for (std::size_t i = 0; i < N; ++i)
            {
                if (i == k)
                {
                    continue;
                }

                double multiplier = temp_matrix[i][k] / -temp_matrix[k][k];
                for (std::size_t j = 0; j < N; ++j)
                {
                    temp_matrix[i][j] += temp_matrix[k][j] * multiplier;
                    result_matrix[i][j] += result_matrix[k][j] * multiplier;
                }
            }

            processed_columns.insert(k);
        }
    }

    return result_matrix;
}

template<typename T, std::size_t N>
void SquareMatrix<T, N>::randomize(int min, int max)
{
    for (std::size_t i = 0; i < N; ++i)
    {
        for (std::size_t j = 0; j < N; ++j)
        {
            m_matrix[i][j] = std::rand() % (max - min + 1) + min;
        }
    }
}

template<typename T, std::size_t N>
void SquareMatrix<T, N>::print()
{
    for (std::size_t i = 0; i < N; ++i)
    {
        for (std::size_t j = 0; j < N; ++j)
        {
            std::cout << std::setprecision(2)
                      << std::setw(12)
                      << m_matrix[i][j] << '\t';
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

template<typename T, std::size_t N>
SquareMatrix<T, N> SquareMatrix<T, N>::operator*
(
    const SquareMatrix<T, N>& matrix_2
)
{
    auto result_matrix = SquareMatrix<T, N>();

    for (std::size_t i1 = 0; i1 < N; ++i1)
    {
        for (std::size_t j = 0; j < N; ++j)
        {
            T sum = 0;

            for (std::size_t i2 = 0; i2 < N; ++i2)
            {
                sum += m_matrix[i1][i2] * matrix_2[i2][j];
            }

            if (global::is_nearly_equal(sum, 0))
            {
                sum = 0;
            }

            result_matrix[i1][j] = sum;
        }
    }

    return result_matrix;
}

#endif // SQUARE_MATRIX_H
