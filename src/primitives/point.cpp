#include "point.h"

#include <stdexcept>

Point3::Point3() : coordinates_{0} {
}

Point3::Point3(long double x, long double y, long double z) : coordinates_{x, y, z} {
}

Point3::Point3(std::initializer_list<long double> list) {
    if (list.size() != 3) {
        throw std::runtime_error("Initializer list size mismatch");
    }
    coordinates_[0] = *list.begin();
    coordinates_[1] = *(list.begin() + 1);
    coordinates_[2] = *(list.begin() + 2);
}

Point3::Point3(const Vector3& vector) {
    for (unsigned i = 0; i < 3; ++i) {
        coordinates_[i] = vector(i, 0);
    }
}

Vector3 Point3::ToVector() const {
    Vector3 result;
    for (unsigned i = 0; i < 3; ++i) {
        result(i, 0) = coordinates_[i];
    }
    return result;
}

std::array<long double, 3> Point3::GetCoordinates() const {
    return coordinates_;
}

long double Point3::operator()(unsigned coordinate) const {
    return coordinates_[coordinate];
}

Point3& Point3::operator+=(const Vector3& vector) {
    for (unsigned i = 0; i < 3; ++i) {
        coordinates_[i] += vector(i, 0);
    }
    return *this;
}

Point3 operator+(const Point3& point, const Vector3& vector) {
    Point3 result = point;
    result += vector;
    return result;
}

Point3& Point3::operator-=(const Vector3& vector) {
    for (unsigned i = 0; i < 3; ++i) {
        coordinates_[i] -= vector(i, 0);
    }
    return *this;
}

Point3 operator-(const Point3& point, const Vector3& vector) {
    Point3 result = point;
    result -= vector;
    return result;
}
