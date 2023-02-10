#pragma once

#include "point.h"

class Vertex {
public:
    Vertex();
    Vertex(const Point3& point);
    Vertex(const Point3& point, uint32_t color);

    void SetPoint(const Point3& point);
    void SetColor(uint32_t color);
    void SetColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);

    Point3 GetPoint() const;
    uint32_t GetColor() const;
    uint8_t GetColorPart(unsigned position) const;

private:
    Point3 point_;
    uint32_t color_;
};