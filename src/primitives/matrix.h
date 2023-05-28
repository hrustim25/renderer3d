#pragma once

#include <array>
#include <stdexcept>

template <unsigned int N, unsigned int M>
class Matrix {
public:
    Matrix() : data_{0} {
    }
    Matrix(const std::array<std::array<long double, M>, N>& data) : data_(data) {
    }
    Matrix(std::initializer_list<long double> list) {
        if (list.size() != N * M) {
            throw std::runtime_error("Initializer list size mismatch");
        }
        for (unsigned int i = 0; i < N; ++i) {
            for (unsigned int j = 0; j < M; ++j) {
                data_[i][j] = *(list.begin() + i * M + j);
            }
        }
    }

    long double& operator()(unsigned int i) {
        return data_[i][0];
    }

    long double operator()(unsigned int i) const {
        return data_[i][0];
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
                data_[i][j] *= coefficient;
            }
        }
        return *this;
    }

    template <unsigned int X, unsigned int Y>
    friend Matrix<X, Y> operator*(const Matrix<X, Y>& matrix, long double coefficient);

    Matrix& operator/=(long double coefficient) {
        for (unsigned int i = 0; i < N; ++i) {
            for (unsigned int j = 0; j < M; ++j) {
                data_[i][j] /= coefficient;
            }
        }
        return *this;
    }

    template <unsigned int X, unsigned int Y>
    friend Matrix<X, Y> operator/(const Matrix<X, Y>& matrix, long double coefficient);

    Matrix& operator*=(const Matrix& other) {
        if (N != M) {
            throw std::runtime_error("Matrix sizes mismatch");
        }
        std::array<std::array<long double, N>, N> result{0};
        for (unsigned int i = 0; i < N; ++i) {
            for (unsigned int j = 0; j < N; ++j) {
                for (unsigned int k = 0; k < N; ++k) {
                    result[i][j] += data_[i][k] * other.data_[k][j];
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
                data_[i][j] += other.data_[i][j];
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
                data_[i][j] -= other.data_[i][j];
            }
        }
        return *this;
    }

    template <unsigned int X, unsigned int Y>
    friend Matrix<X, Y> operator-(const Matrix<X, Y>& matrix1, const Matrix<X, Y>& matrix2);

    Matrix<M, N> Transpose() const {
        Matrix<M, N> result_matrix;
        for (unsigned int i = 0; i < N; ++i) {
            for (unsigned int j = 0; j < M; ++j) {
                result_matrix(j, i) = (*this)(i, j);
            }
        }
        return result_matrix;
    }

private:
    std::array<std::array<long double, M>, N> data_;
};

template <unsigned int N, unsigned int M>
Matrix<N, M> operator*(const Matrix<N, M>& matrix1, long double coefficient) {
    Matrix result_matrix = matrix1;
    result_matrix *= coefficient;
    return result_matrix;
}

template <unsigned int N, unsigned int M>
Matrix<N, M> operator/(const Matrix<N, M>& matrix1, long double coefficient) {
    Matrix result_matrix = matrix1;
    result_matrix /= coefficient;
    return result_matrix;
}

template <unsigned int N, unsigned int M, unsigned int K>
Matrix<N, K> operator*(const Matrix<N, M>& matrix1, const Matrix<M, K>& matrix2) {
    Matrix<N, K> result_matrix;
    for (unsigned int i = 0; i < N; ++i) {
        for (unsigned int j = 0; j < K; ++j) {
            for (unsigned int k = 0; k < M; ++k) {
                result_matrix.data_[i][j] += matrix1.data_[i][k] * matrix2.data_[k][j];
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
