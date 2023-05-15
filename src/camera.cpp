#include "camera.h"

Camera::Camera() : Pivot(), screen_width_(800), screen_height_(600) {
    view_piramid_width_tan = (long double)screen_width_ / 2 / clip_distance_;
    view_piramid_height_tan = (long double)screen_height_ / 2 / clip_distance_;
}

Camera::Camera(unsigned screen_width, unsigned screen_height)
    : Pivot(), screen_width_(screen_width), screen_height_(screen_height) {
    view_piramid_width_tan = (long double)screen_width_ / 2 / clip_distance_;
    view_piramid_height_tan = (long double)screen_height_ / 2 / clip_distance_;
}

Camera::Camera(const Pivot& pivot, unsigned screen_width, unsigned screen_height)
    : Pivot(pivot), screen_width_(screen_width), screen_height_(screen_height) {
}

Point4 Camera::ProjectToScreen(const Point4& point) const {
    Point4 local_point = ToLocalCoordinates(point);
    long double coefficient = GetClipDistance() / local_point(2);
    long double new_x = GetScreenWidth() / 2 + local_point(0) * coefficient;
    long double new_y = GetScreenHeight() / 2 - local_point(1) * coefficient;
    return {new_x, new_y, local_point(2), 1};
}

long double Camera::GetNearClipDistance() const {
    return near_clip_distance_;
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

long double Camera::GetViewPiramidWidthTan() const {
    return view_piramid_width_tan;
}

long double Camera::GetViewPiramidHeightTan() const {
    return view_piramid_height_tan;
}
