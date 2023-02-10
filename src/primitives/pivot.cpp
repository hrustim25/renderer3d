#include "pivot.h"

#include <cmath>

Pivot::Pivot() : center_(), basis_(CreateIdentityMatrix<3>()) {
}

Pivot::Pivot(const Point3& center) : center_(center), basis_(CreateIdentityMatrix<3>()) {
}

Pivot::Pivot(const Point3& center, const Vector3& basis_vector_1, const Vector3& basis_vector_2,
             const Vector3& basis_vector_3)
    : center_(center) {
    for (unsigned i = 0; i < 3; ++i) {
        basis_(i, 0) = basis_vector_1(i, 0);
        basis_(i, 1) = basis_vector_2(i, 0);
        basis_(i, 2) = basis_vector_3(i, 0);
    }
}

Pivot::Pivot(const Point3& center, const Matrix3& basis) : center_(center), basis_(basis) {
}

void Pivot::Move(const Vector3& vector) {
    center_ = center_ + vector;
}

void Pivot::Rotate(const Matrix3& rotation_matrix) {
    basis_ *= rotation_matrix;
}

Point3 Pivot::ToGlobalCoordinates(const Point3& point) const {
    return basis_.Transpose() * point.ToVector() + center_.ToVector();
}

Point3 Pivot::ToLocalCoordinates(const Point3& point) const {
    return basis_ * (point.ToVector() - center_.ToVector());
}

Matrix3 CreateRotationMatrix(unsigned axis, long double angle) {
    Matrix3 result;
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
    return result;
}
