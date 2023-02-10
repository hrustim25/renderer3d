#pragma once

#include "point.h"

// Coordinate system orthnormal basis and center global coordinates
class Pivot {
public:
    Pivot();
    Pivot(const Point3& center);
    Pivot(const Point3& center, const Vector3& basis_vector_1, const Vector3& basis_vector_2,
          const Vector3& basis_vector_3);
    Pivot(const Point3& center, const Matrix3& basis);

    void Move(const Vector3& vector);
    void Rotate(const Matrix3& rotation_matrix);

    Point3 ToGlobalCoordinates(const Point3& point) const;
    Point3 ToLocalCoordinates(const Point3& point) const;

private:
    Point3 center_;
    Matrix3 basis_;
};

// axis = 0, 1, 2 - XY, XZ, YZ rotation, respectively
Matrix3 CreateRotationMatrix(unsigned axis, long double angle);
