#pragma once

#include "primitives/point.h"
#include "image.h"

#include <vector>

class Space {
public:
    struct Polygon {
        unsigned point_indexes[3];
        unsigned colors_indexes[3];
        unsigned texture_index;
        unsigned tex_coords_indexes[3];
        unsigned normal_indexes[3];
    };

public:
    Space();
    Space(unsigned expected_size);

    void AddPoint(const Point4& point);
    void AddColor(uint32_t color);
    void AddTextureCoords(long double tex_x, long double tex_y);
    void AddTexture(const Image& texture);
    void AddNormal(const Vector4& normal);
    void AddPolygon(unsigned point_index1, unsigned point_index2, unsigned point_index3);

    void SetPolygonColors(unsigned polygon_index, unsigned color_index1, unsigned color_index2,
                          unsigned color_index3);
    void SetPolygonTextureCoords(unsigned polygon_index, unsigned tex_coords_index1,
                                 unsigned tex_coords_index2, unsigned tex_coords_index3);
    void SetPolygonTexture(unsigned polygon_index, unsigned texture_index);
    void SetPolygonNormals(unsigned polygon_index, unsigned normal_index1, unsigned normal_index2,
                           unsigned normal_index3);

    void SetPoint(unsigned index, const Point4& point);
    Point4 GetPoint(unsigned index) const;

    void SetColor(unsigned index, uint32_t color);
    uint32_t GetColor(unsigned index) const;

    void SetTexCoords(unsigned index, long double tex_x, long double tex_y);
    std::pair<long double, long double> GetTexCoords(unsigned index) const;

    void SetTexture(unsigned index, const Image& texture);
    const Image* GetTexturePointer(unsigned index) const;

    void SetNormal(unsigned index, const Vector4& normal);
    Vector4 GetNormal(unsigned index) const;

    Polygon GetPolygon(unsigned index) const;

    void TransformPoint(unsigned index, const Matrix4& transformation_matrix);
    void TransformNormal(unsigned index, const Matrix4& transformation_matrix);

    size_t GetPointCount() const;
    size_t GetColorCount() const;
    size_t GetTexCoordsCount() const;
    size_t GetTextureCount() const;
    size_t GetNormalCount() const;
    size_t GetPolygonCount() const;

private:
    Vector4 NormalizeVector(const Vector4& vec);

    std::vector<Point4> points_;
    std::vector<uint32_t> colors_;
    std::vector<std::pair<long double, long double>> tex_coords_;
    std::vector<Image> textures_;
    std::vector<Vector4> normals_;
    std::vector<Polygon> polygons_;
};
