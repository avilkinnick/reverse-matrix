#include <cmath>
#include <ctime>
#include <iostream>
#include <random>

#include "SquareMatrix.h"

bool is_zero(double a);

// Создать динамическую квадратную матрицу
template <typename T>
T** create_square_matrix(int size);

// Заполнить квадратную матрицу случайными числами от min до max включительно
template <typename T>
void randomize_square_matrix(T** matrix, int size, int min, int max);

// Вывести квадратную матрицу на экран
template <typename T>
void print_square_matrix(T** matrix, int size);

// Создать квадратную подматрицу
// (с вычеркнутой строкой и столбцом из оригинальной матрицы)
template <typename T>
T** create_square_submatrix(T** matrix, int size, int row, int column);

// Вычислить детерминант
template <typename T>
T calculate_determinant(T** matrix, int size);

// Создать матрицу алгебраических дополнений
template <typename T>
T** create_cofactor_matrix(T** matrix, int size);

// Создать транспонированную матрицу алгебраических дополнений
template <typename T>
T** create_adjugate_matrix(T** matrix, int size);

// Создать квадратную матрицу - произведение квадратной матрицы и скаляра
template <typename T>
T** multiplication_of_square_matrix_by_scalar(T** matrix, int size, double scalar);

// Создать квадратную матрицу - произведение 2-х квадратных матриц одного размера
template <typename T>
T** multiplication_of_square_matrixes(T** matrix_1, T** matrix_2, int size);

// Удалить динамическую квадратную матрицу
template <typename T>
void delete_square_matrix(T**& matrix, int size);

int main()
{
    srand(time(nullptr));

    constexpr std::size_t n = 500;

    auto matrix = SquareMatrix<double, n>();
    matrix.randomize(-9, 9);
    matrix.print();

    auto inverse_matrix = SquareMatrix<double, n>::inverse_matrix(matrix);
    inverse_matrix.print();

    auto identity_matrix = matrix * inverse_matrix;
    identity_matrix.print();
}

bool is_zero(double a)
{
    static constexpr double epsilon = 0.0000000001;
    return std::abs(a) < epsilon;
}

template <typename T>
T** create_square_matrix(int size)
{
    T** matrix = new T*[size];
    for (int i = 0; i < size; ++i)
    {
        matrix[i] = new T[size];
    }
    return matrix;
}

template <typename T>
void randomize_square_matrix(T** matrix, int size, int min, int max)
{
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            matrix[i][j] = std::rand() % (max - min + 1) + min;
        }
    }
}

template <typename T>
void print_square_matrix(T** matrix, int size)
{
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            std::cout << matrix[i][j] << '\t';
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

template <typename T>
T** create_square_submatrix(T** matrix, int size, int row, int column)
{
    T** submatrix = create_square_matrix<T>(size - 1);
    int submatrix_row = 0;

    for (int i = 0; i < size; ++i)
    {
        if (i != row)
        {
            int submatrix_column = 0;
            for (int j = 0; j < size; ++j)
            {
                if (j != column)
                {
                    submatrix[submatrix_row][submatrix_column++] = matrix[i][j];
                }
            }
            ++submatrix_row;
        }
    }

    return submatrix;
}

template <typename T>
T calculate_determinant(T** matrix, int size)
{
    if (size == 1)
    {
        return **matrix;
    }
    else
    {
        T sum = 0;
        int sign = 1;

        // Определитель по первой строке
        for (int j = 0; j < size; ++j)
        {
            T** submatrix = create_square_submatrix<T>(matrix, size, 0, j);

            sum += sign * matrix[0][j]
                * calculate_determinant(submatrix, size - 1);

            delete_square_matrix(submatrix, size - 1);

            sign = -sign;
        }

        return sum;
    }
}

template <typename T>
T** create_cofactor_matrix(T** matrix, int size)
{
    T** cofactor_matrix = create_square_matrix<T>(size);

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            int sign = ((i + j) % 2 == 0) ? 1 : -1;
            T** submatrix = create_square_submatrix<T>(matrix, size, i, j);

            cofactor_matrix[i][j] = sign
                * calculate_determinant(submatrix, size - 1);

            delete_square_matrix(submatrix, size - 1);
        }
    }

    return cofactor_matrix;
}

template <typename T>
T** create_adjugate_matrix(T** matrix, int size)
{
    T** adjugate_matrix = create_square_matrix<T>(size);

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            adjugate_matrix[i][j] = matrix[j][i];
        }
    }

    return adjugate_matrix;
}

template <typename T>
T** multiplication_of_square_matrix_by_scalar(T** matrix, int size, double scalar)
{
    T** result_matrix = create_square_matrix<T>(size);

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            result_matrix[i][j] = matrix[i][j] * scalar;
        }
    }

    return result_matrix;
}

template <typename T>
T** multiplication_of_square_matrixes(T** matrix_1, T** matrix_2, int size)
{
    T** result_matrix = create_square_matrix<T>(size);

    for (int i1 = 0; i1 < size; ++i1)
    {
        for (int j = 0; j < size; ++j)
        {
            T sum = 0;
            for (int i2 = 0; i2 < size; ++i2)
            {
                sum += matrix_1[i1][i2] * matrix_2[i2][j];
            }
            result_matrix[i1][j] = is_zero(sum) ? 0 : sum;
        }
    }

    return result_matrix;
}

template <typename T>
void delete_square_matrix(T**& matrix, int size)
{
    for (int i = 0; i < size; ++i)
    {
        delete[] matrix[i];
        matrix[i] = nullptr;
    }
    delete[] matrix;
    matrix = nullptr;
}
