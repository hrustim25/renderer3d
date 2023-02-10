#pragma once

#include "primitives/point.h"

#include <vector>

class Space {
public:
    struct Polygon {
        unsigned point_indexes[3];
        unsigned colors_indexes[3];
    };

public:
    Space();
    Space(unsigned expected_size);

    void AddPoint(const Point3& point);
    void AddColor(uint32_t color);
    void AddPolygon(unsigned point_index1, unsigned point_index2, unsigned point_index3);

    void SetPolygonColors(unsigned polygon_index, unsigned color_index1, unsigned color_index2,
                          unsigned color_index3);

    void SetPoint(unsigned index, const Point3& point);
    Point3 GetPoint(unsigned index) const;

    void SetColor(unsigned index, uint32_t color);
    uint32_t GetColor(unsigned index) const;

    Polygon GetPolygon(unsigned index) const;

    size_t GetPointCount() const;
    size_t GetColorCount() const;
    size_t GetPolygonCount() const;

private:
    std::vector<Point3> points_;
    std::vector<uint32_t> colors_;
    std::vector<Polygon> polygons_;
};
