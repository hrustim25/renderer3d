#pragma once

#include "primitives/pivot.h"

class Camera : public Pivot {
public:
    Camera();
    Camera(unsigned screen_width, unsigned screen_height);
    Camera(const Pivot& pivot, unsigned screen_width, unsigned screen_height);

    Point4 ProjectToScreen(const Point4& point) const;

    long double GetNearClipDistance() const;
    long double GetClipDistance() const;
    unsigned GetScreenWidth() const;
    unsigned GetScreenHeight() const;
    long double GetViewPiramidWidthTan() const;
    long double GetViewPiramidHeightTan() const;

private:
    long double near_clip_distance_ = 0.01;
    long double clip_distance_ = 800.0;
    unsigned screen_width_;
    unsigned screen_height_;
    long double view_piramid_width_tan;
    long double view_piramid_height_tan;
};
