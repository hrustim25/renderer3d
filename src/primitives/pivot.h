#pragma once

#include "point.h"

// Coordinate system orthnormal basis and center global coordinates
class Pivot {
public:
    Pivot();
    Pivot(const Point4& center);
    Pivot(const Point4& center, const Vector4& basis_vector_1, const Vector4& basis_vector_2,
          const Vector4& basis_vector_3);
    Pivot(const Point4& center, const Matrix4& basis);

    void Transform(const Matrix4& transformation_matrix);

    Point4 ToGlobalCoordinates(const Point4& point) const;
    Point4 ToLocalCoordinates(const Point4& point) const;

private:
    Point4 center_;
    Matrix4 basis_;
};

// axis = 0, 1, 2 - XY, XZ, YZ rotation, respectively
Matrix4 CreateRotationMatrix(unsigned axis, long double angle);

Matrix4 CreateMoveMatrix(long double dx, long double dy, long double dz);
