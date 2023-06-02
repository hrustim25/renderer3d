#pragma once

#include "primitives/pivot.h"

namespace rend {

static constexpr long double default_near_clip_distance = 0.01;
static constexpr long double default_clip_distance = 1000.0;

class Camera : public Pivot {
public:
    Camera(unsigned int screen_width, unsigned int screen_height);
    Camera(const Pivot& pivot, unsigned int screen_width, unsigned int screen_height);

    Point4 ProjectToScreen(const Point4& point) const;

    long double GetNearClipDistance() const;
    long double GetClipDistance() const;
    long double GetViewPiramidWidthTan() const;
    long double GetViewPiramidHeightTan() const;

private:
    const long double near_clip_distance_ = default_near_clip_distance;
    const long double clip_distance_ = default_clip_distance;
    const unsigned int screen_width_;
    const unsigned int screen_height_;
    const long double view_piramid_width_tan_;
    const long double view_piramid_height_tan_;
};

}  // namespace rend
