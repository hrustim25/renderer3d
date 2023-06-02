#pragma once

#include <array>
#include <stdexcept>

#include <cassert>

namespace rend {

template <unsigned int N, unsigned int M>
class Matrix {
public:
    Matrix() : data_{0} {
    }
    Matrix(const std::array<std::array<long double, M>, N>& data) : data_(data) {
    }
    Matrix(std::initializer_list<long double> list) {
        assert(list.size() == N * M);
        for (unsigned int i = 0; i < N; ++i) {
            for (unsigned int j = 0; j < M; ++j) {
                data_[i][j] = *(list.begin() + i * M + j);
            }
        }
    }

    long double& operator()(unsigned int i) {
        static_assert(N == 1 || M == 1);
        if constexpr (N == 1) {
            return data_[0][i];
        } else {
            return data_[i][0];
        }
    }

    long double operator()(unsigned int i) const {
        static_assert(N == 1 || M == 1);
        if constexpr (N == 1) {
            return data_[0][i];
        } else {
            return data_[i][0];
        }
    }

    long double& operator()(unsigned int i, unsigned int j) {
        return data_[i][j];
    }

    long double operator()(unsigned int i, unsigned int j) const {
        return data_[i][j];
    }

    Matrix& operator*=(long double coefficient) {
        for (unsigned int i = 0; i < N; ++i) {
            for (unsigned int j = 0; j < M; ++j) {
                (*this)(i, j) *= coefficient;
            }
        }
        return *this;
    }

    friend Matrix operator*(const Matrix& matrix, long double coefficient) {
        Matrix result = matrix;
        result *= coefficient;
        return result;
    }

    friend Matrix operator*(long double coefficient, const Matrix& matrix) {
        return matrix * coefficient;
    }

    Matrix& operator/=(long double coefficient) {
        for (unsigned int i = 0; i < N; ++i) {
            for (unsigned int j = 0; j < M; ++j) {
                (*this)(i, j) /= coefficient;
            }
        }
        return *this;
    }

    friend Matrix operator/(const Matrix& matrix, long double coefficient) {
        Matrix result = matrix;
        result /= coefficient;
        return result;
    }

    Matrix& operator*=(const Matrix& other) {
        static_assert(N == M);
        std::array<std::array<long double, N>, N> result{0};
        for (unsigned int i = 0; i < N; ++i) {
            for (unsigned int j = 0; j < N; ++j) {
                for (unsigned int k = 0; k < N; ++k) {
                    result[i][j] += (*this)(i, k) * other(k, j);
                }
            }
        }
        data_ = result;
        return *this;
    }

    template <unsigned int X, unsigned int Y, unsigned int Z>
    friend Matrix<X, Z> operator*(const Matrix<X, Y>& matrix1, const Matrix<Y, Z>& matrix2);

    Matrix& operator+=(const Matrix& other) {
        for (unsigned int i = 0; i < N; ++i) {
            for (unsigned int j = 0; j < M; ++j) {
                (*this)(i, j) += other(i, j);
            }
        }
        return *this;
    }

    template <unsigned int X, unsigned int Y>
    friend Matrix<X, Y> operator+(const Matrix<X, Y>& matrix1, const Matrix<X, Y>& matrix2);

    Matrix& operator-() {
        *this *= -1;
        return *this;
    }
    Matrix& operator-=(const Matrix& other) {
        for (unsigned int i = 0; i < N; ++i) {
            for (unsigned int j = 0; j < M; ++j) {
                (*this)(i, j) -= other(i, j);
            }
        }
        return *this;
    }

    template <unsigned int X, unsigned int Y>
    friend Matrix<X, Y> operator-(const Matrix<X, Y>& matrix1, const Matrix<X, Y>& matrix2);

    static Matrix<M, N> Transpose(const Matrix<N, M>& matrix) {
        Matrix<M, N> result_matrix;
        for (unsigned int i = 0; i < N; ++i) {
            for (unsigned int j = 0; j < M; ++j) {
                result_matrix(j, i) = matrix(i, j);
            }
        }
        return result_matrix;
    }

private:
    std::array<std::array<long double, M>, N> data_;
};

template <unsigned int N, unsigned int M, unsigned int K>
Matrix<N, K> operator*(const Matrix<N, M>& matrix1, const Matrix<M, K>& matrix2) {
    Matrix<N, K> result_matrix;
    for (unsigned int i = 0; i < N; ++i) {
        for (unsigned int j = 0; j < K; ++j) {
            for (unsigned int k = 0; k < M; ++k) {
                result_matrix(i, j) += matrix1(i, k) * matrix2(k, j);
            }
        }
    }
    return result_matrix;
}

template <unsigned int N, unsigned int M>
Matrix<N, M> operator+(const Matrix<N, M>& matrix1, const Matrix<N, M>& matrix2) {
    Matrix result_matrix = matrix1;
    result_matrix += matrix2;
    return result_matrix;
}

template <unsigned int N, unsigned int M>
Matrix<N, M> operator-(const Matrix<N, M>& matrix1, const Matrix<N, M>& matrix2) {
    Matrix result_matrix = matrix1;
    result_matrix -= matrix2;
    return result_matrix;
}

using Matrix4 = Matrix<4, 4>;
using Vector4 = Matrix<4, 1>;

template <unsigned int N>
Matrix<N, N> CreateIdentityMatrix() {
    Matrix<N, N> result;
    for (unsigned int i = 0; i < N; ++i) {
        result(i, i) = 1;
    }
    return result;
}

}  // namespace rend
