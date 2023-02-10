#pragma once

#include "primitives/pivot.h"

class Camera : public Pivot {
public:
    Camera();
    Camera(unsigned screen_width, unsigned screen_height);
    Camera(const Pivot& pivot, unsigned screen_width, unsigned screen_height);

    Point3 ProjectToScreen(const Point3& point) const;

    long double GetClipDistance() const;
    unsigned GetScreenWidth() const;
    unsigned GetScreenHeight() const;

private:
    long double clip_distance_ = 800.0;
    unsigned screen_width_;
    unsigned screen_height_;
};