#include "space.h"

#include <cmath>

Space::Space() {
}

Space::Space(unsigned expected_size) {
    points_.reserve(expected_size);
    colors_.reserve(expected_size);
    polygons_.reserve(expected_size);
}

void Space::AddPoint(const Point4& point) {
    points_.push_back(point);
}

void Space::AddColor(uint32_t color) {
    colors_.push_back(color);
}

Vector4 Space::NormalizeVector(const Vector4& vec) {
    long double length = std::sqrt(vec(0) * vec(0) + vec(1) * vec(1) + vec(2) * vec(2));
    if (length == 0) {
        length = 1;
    }
    return {vec(0) / length, vec(1) / length, vec(2) / length, vec(3)};
}

void Space::AddNormal(const Vector4& normal) {
    normals_.push_back(NormalizeVector(normal));
}

void Space::AddTextureCoords(long double tex_x, long double tex_y) {
    tex_coords_.push_back(std::make_pair(tex_x, tex_y));
}

void Space::AddTexture(const Image& texture) {
    textures_.push_back(texture);
}

void Space::AddPolygon(unsigned point_index1, unsigned point_index2, unsigned point_index3) {
    Polygon polygon;
    polygon.point_indexes[0] = point_index1;
    polygon.point_indexes[1] = point_index2;
    polygon.point_indexes[2] = point_index3;
    polygon.colors_indexes[0] = polygon.colors_indexes[1] = polygon.colors_indexes[2] = 0;
    polygon.texture_index = 0;
    polygon.tex_coords_indexes[0] = polygon.tex_coords_indexes[1] = polygon.tex_coords_indexes[2] =
        0;
    polygon.normal_indexes[0] = polygon.normal_indexes[1] = polygon.normal_indexes[2] = 0;
    polygons_.push_back(polygon);
}

void Space::SetPolygonColors(unsigned polygon_index, unsigned color_index1, unsigned color_index2,
                             unsigned color_index3) {
    polygons_[polygon_index].colors_indexes[0] = color_index1;
    polygons_[polygon_index].colors_indexes[1] = color_index2;
    polygons_[polygon_index].colors_indexes[2] = color_index3;
}

void Space::SetPolygonTextureCoords(unsigned polygon_index, unsigned tex_coords_index1,
                                    unsigned tex_coords_index2, unsigned tex_coords_index3) {
    polygons_[polygon_index].tex_coords_indexes[0] = tex_coords_index1;
    polygons_[polygon_index].tex_coords_indexes[1] = tex_coords_index2;
    polygons_[polygon_index].tex_coords_indexes[2] = tex_coords_index3;
}

void Space::SetPolygonTexture(unsigned polygon_index, unsigned texture_index) {
    polygons_[polygon_index].texture_index = texture_index;
}

void Space::SetPolygonNormals(unsigned polygon_index, unsigned normal_index1,
                              unsigned normal_index2, unsigned normal_index3) {
    polygons_[polygon_index].normal_indexes[0] = normal_index1;
    polygons_[polygon_index].normal_indexes[1] = normal_index2;
    polygons_[polygon_index].normal_indexes[2] = normal_index3;
}

void Space::SetPoint(unsigned index, const Point4& point) {
    points_[index] = point;
}

void Space::SetColor(unsigned index, uint32_t color) {
    colors_[index] = color;
}

void Space::SetTexCoords(unsigned index, long double tex_x, long double tex_y) {
    tex_coords_[index].first = tex_x;
    tex_coords_[index].second = tex_y;
}

void Space::SetTexture(unsigned index, const Image& texture) {
    textures_[index] = texture;
}

void Space::SetNormal(unsigned index, const Vector4& normal) {
    normals_[index] = NormalizeVector(normal);
}

Point4 Space::GetPoint(unsigned index) const {
    return points_[index];
}

uint32_t Space::GetColor(unsigned index) const {
    return colors_[index];
}

std::pair<long double, long double> Space::GetTexCoords(unsigned index) const {
    return tex_coords_[index];
}

const Image* Space::GetTexturePointer(unsigned index) const {
    return &textures_[index];
}

Vector4 Space::GetNormal(unsigned index) const {
    return normals_[index];
}

Space::Polygon Space::GetPolygon(unsigned index) const {
    return polygons_[index];
}

void Space::TransformPoint(unsigned index, const Matrix4& transformation_matrix) {
    SetPoint(index, transformation_matrix * GetPoint(index));
}

void Space::TransformNormal(unsigned index, const Matrix4& transformation_matrix) {
    Vector4 result = NormalizeVector(transformation_matrix * GetNormal(index));
    result(3) = 0;
    SetNormal(index, result);
}

size_t Space::GetPointCount() const {
    return points_.size();
}

size_t Space::GetColorCount() const {
    return colors_.size();
}

size_t Space::GetTexCoordsCount() const {
    return tex_coords_.size();
}

size_t Space::GetTextureCount() const {
    return textures_.size();
}

size_t Space::GetNormalCount() const {
    return normals_.size();
}

size_t Space::GetPolygonCount() const {
    return polygons_.size();
}
