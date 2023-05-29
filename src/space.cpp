#include "space.h"

#include <cmath>

namespace rend {

Space::Space() : textures_(1), normals_{{-1, 0, 0, 0}} {
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

void Space::AddPolygon(unsigned int point_index1, unsigned int point_index2,
                       unsigned int point_index3) {
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

void Space::SetPolygonColors(unsigned int polygon_index, unsigned int color_index1,
                             unsigned int color_index2, unsigned int color_index3) {
    polygons_[polygon_index].colors_indexes[0] = color_index1;
    polygons_[polygon_index].colors_indexes[1] = color_index2;
    polygons_[polygon_index].colors_indexes[2] = color_index3;
}

void Space::SetPolygonTextureCoords(unsigned int polygon_index, unsigned int tex_coords_index1,
                                    unsigned int tex_coords_index2,
                                    unsigned int tex_coords_index3) {
    polygons_[polygon_index].tex_coords_indexes[0] = tex_coords_index1;
    polygons_[polygon_index].tex_coords_indexes[1] = tex_coords_index2;
    polygons_[polygon_index].tex_coords_indexes[2] = tex_coords_index3;
}

void Space::SetPolygonTexture(unsigned int polygon_index, unsigned int texture_index) {
    polygons_[polygon_index].texture_index = texture_index;
}

void Space::SetPolygonNormals(unsigned int polygon_index, unsigned int normal_index1,
                              unsigned int normal_index2, unsigned int normal_index3) {
    polygons_[polygon_index].normal_indexes[0] = normal_index1;
    polygons_[polygon_index].normal_indexes[1] = normal_index2;
    polygons_[polygon_index].normal_indexes[2] = normal_index3;
}

void Space::SetPoint(unsigned int index, const Point4& point) {
    points_[index] = point;
}

void Space::SetColor(unsigned int index, uint32_t color) {
    colors_[index] = color;
}

void Space::SetTexCoords(unsigned int index, long double tex_x, long double tex_y) {
    tex_coords_[index].first = tex_x;
    tex_coords_[index].second = tex_y;
}

void Space::SetTexture(unsigned int index, const Image& texture) {
    textures_[index] = texture;
}

void Space::SetNormal(unsigned int index, const Vector4& normal) {
    normals_[index] = NormalizeVector(normal);
}

const Point4& Space::GetPoint(unsigned int index) const {
    return points_[index];
}

uint32_t Space::GetColor(unsigned int index) const {
    return colors_[index];
}

std::pair<long double, long double> Space::GetTexCoords(unsigned int index) const {
    return tex_coords_[index];
}

const Image* Space::GetTexturePointer(unsigned int index) const {
    return &textures_[index];
}

const Vector4& Space::GetNormal(unsigned int index) const {
    return normals_[index];
}

const Space::Polygon& Space::GetPolygon(unsigned int index) const {
    return polygons_[index];
}

void Space::TransformAll(const Matrix4& transformation_matrix) {
    for (size_t i = 0; i < GetPointCount(); ++i) {
        SetPoint(i, transformation_matrix * GetPoint(i));
    }
    for (size_t i = 1; i < GetNormalCount(); ++i) {
        Vector4 result = NormalizeVector(transformation_matrix * GetNormal(i));
        result(3) = 0;
        SetNormal(i, result);
    }
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

}  // namespace rend
