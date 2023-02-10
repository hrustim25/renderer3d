#include "camera.h"

Camera::Camera() : Pivot(), screen_width_(800), screen_height_(600) {
}

Camera::Camera(unsigned screen_width, unsigned screen_height)
    : Pivot(), screen_width_(screen_width), screen_height_(screen_height) {
}

Camera::Camera(const Pivot& pivot, unsigned screen_width, unsigned screen_height)
    : Pivot(pivot), screen_width_(screen_width), screen_height_(screen_height) {
}

Point3 Camera::ProjectToScreen(const Point3& point) const {
    Point3 local_point = ToLocalCoordinates(point);
    if (local_point.GetCoordinates()[2] == 0) {
        // return Point3(-1, -1, -1);
    }
    long double coefficient = GetClipDistance() / local_point(2);
    long double new_x = GetScreenWidth() / 2 + local_point(0) * coefficient;
    long double new_y = GetScreenHeight() / 2 - local_point(1) * coefficient;
    if (new_x < 0 || new_x > GetScreenWidth() || new_y < 0 || new_y > GetScreenHeight()) {
        // return Point3(-1, -1, -1);
    }
    return Point3(new_x, new_y, local_point(2));
}

long double Camera::GetClipDistance() const {
    return clip_distance_;
}

unsigned Camera::GetScreenWidth() const {
    return screen_width_;
}

unsigned Camera::GetScreenHeight() const {
    return screen_height_;
}
