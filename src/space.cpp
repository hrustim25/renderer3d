#include "space.h"

Space::Space() {
}

Space::Space(unsigned expected_size) {
    points_.reserve(expected_size);
    colors_.reserve(expected_size);
    polygons_.reserve(expected_size);
}

void Space::AddPoint(const Point3& point) {
    points_.push_back(point);
}

void Space::AddColor(uint32_t color) {
    colors_.push_back(color);
}

void Space::AddPolygon(unsigned point_index1, unsigned point_index2, unsigned point_index3) {
    Polygon polygon;
    polygon.point_indexes[0] = point_index1;
    polygon.point_indexes[1] = point_index2;
    polygon.point_indexes[2] = point_index3;
    polygons_.push_back(polygon);
}

void Space::SetPolygonColors(unsigned polygon_index, unsigned color_index1, unsigned color_index2,
                             unsigned color_index3) {
    polygons_[polygon_index].colors_indexes[0] = color_index1;
    polygons_[polygon_index].colors_indexes[1] = color_index2;
    polygons_[polygon_index].colors_indexes[2] = color_index3;
}

void Space::SetPoint(unsigned index, const Point3& point) {
    points_[index] = point;
}

Point3 Space::GetPoint(unsigned index) const {
    return points_[index];
}

void Space::SetColor(unsigned index, uint32_t color) {
    colors_[index] = color;
}

uint32_t Space::GetColor(unsigned index) const {
    return colors_[index];
}

Space::Polygon Space::GetPolygon(unsigned index) const {
    return polygons_[index];
}

size_t Space::GetPointCount() const {
    return points_.size();
}

size_t Space::GetColorCount() const {
    return colors_.size();
}
size_t Space::GetPolygonCount() const {
    return polygons_.size();
}
