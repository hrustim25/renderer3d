#include "pivot.h"

#include <cmath>

Pivot::Pivot() : center_(), basis_(CreateIdentityMatrix<4>()) {
}

Pivot::Pivot(const Point4& center) : center_(center), basis_(CreateIdentityMatrix<4>()) {
}

Pivot::Pivot(const Point4& center, const Vector4& basis_vector_1, const Vector4& basis_vector_2,
             const Vector4& basis_vector_3)
    : center_(center) {
    for (unsigned int i = 0; i < 4; ++i) {
        basis_(i, 0) = basis_vector_1(i, 0);
        basis_(i, 1) = basis_vector_2(i, 0);
        basis_(i, 2) = basis_vector_3(i, 0);
    }
    basis_(3, 3) = 1;
}

Pivot::Pivot(const Point4& center, const Matrix4& basis) : center_(center), basis_(basis) {
}

void Pivot::Transform(const Matrix4& transformation_matrix) {
    center_ = transformation_matrix * center_;
    basis_ = transformation_matrix * basis_;
}

Point4 Pivot::ToGlobalCoordinates(const Point4& point) const {
    return basis_.Transpose() * point + center_;
}

Point4 Pivot::ToLocalCoordinates(const Point4& point) const {
    return basis_ * (point - center_);
}

Matrix4 CreateRotationMatrix(unsigned int axis, long double angle) {
    Matrix4 result;
    if (axis == 0) {
        result(0, 0) = std::cos(angle);
        result(1, 0) = std::sin(angle);
        result(0, 1) = -std::sin(angle);
        result(1, 1) = std::cos(angle);
        result(2, 2) = 1;
    } else if (axis == 1) {
        result(0, 0) = std::cos(angle);
        result(2, 0) = std::sin(angle);
        result(0, 2) = -std::sin(angle);
        result(2, 2) = std::cos(angle);
        result(1, 1) = 1;
    } else {
        result(1, 1) = std::cos(angle);
        result(2, 1) = std::sin(angle);
        result(1, 2) = -std::sin(angle);
        result(2, 2) = std::cos(angle);
        result(0, 0) = 1;
    }
    result(3, 3) = 1;
    return result;
}

Matrix4 CreateMoveMatrix(long double dx, long double dy, long double dz) {
    Matrix4 result = CreateIdentityMatrix<4>();
    result(0, 3) = dx;
    result(1, 3) = dy;
    result(2, 3) = dz;
    return result;
}
