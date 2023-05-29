#include "camera.h"

namespace rend {

Camera::Camera(unsigned int screen_width, unsigned int screen_height)
    : Pivot(),
      screen_width_(screen_width),
      screen_height_(screen_height),
      view_piramid_width_tan_(static_cast<long double>(screen_width_) / 2 / clip_distance_),
      view_piramid_height_tan_(static_cast<long double>(screen_height_) / 2 / clip_distance_) {
}

Camera::Camera(const Pivot& pivot, unsigned int screen_width, unsigned int screen_height)
    : Pivot(pivot),
      screen_width_(screen_width),
      screen_height_(screen_height),
      view_piramid_width_tan_(static_cast<long double>(screen_width_) / 2 / clip_distance_),
      view_piramid_height_tan_(static_cast<long double>(screen_height_) / 2 / clip_distance_) {
}

Point4 Camera::ProjectToScreen(const Point4& point) const {
    long double coefficient = GetClipDistance() / point(2);
    long double new_x = static_cast<long double>(screen_width_) / 2 + point(0) * coefficient;
    long double new_y = static_cast<long double>(screen_height_) / 2 - point(1) * coefficient;
    return {new_x, new_y, point(2), 1};
}

long double Camera::GetNearClipDistance() const {
    return near_clip_distance_;
}

long double Camera::GetClipDistance() const {
    return clip_distance_;
}

long double Camera::GetViewPiramidWidthTan() const {
    return view_piramid_width_tan_;
}

long double Camera::GetViewPiramidHeightTan() const {
    return view_piramid_height_tan_;
}

}  // namespace rend
