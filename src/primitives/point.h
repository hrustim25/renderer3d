#pragma once

#include <array>

#include "matrix.h"

// 3D point
class Point3 {
public:
    Point3();
    Point3(long double x, long double y, long double z);
    Point3(std::initializer_list<long double> list);
    Point3(const Vector3& vector);

    Vector3 ToVector() const;

    std::array<long double, 3> GetCoordinates() const;

    long double operator()(unsigned coordinate) const;

    Point3& operator+=(const Vector3& vector);
    friend Point3 operator+(const Point3& point, const Vector3& vector);

    Point3& operator-=(const Vector3& vector);
    friend Point3 operator-(const Point3& point, const Vector3& vector);

private:
    std::array<long double, 3> coordinates_;
};
