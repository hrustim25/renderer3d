#pragma once

#include "primitives/point.h"
#include "image.h"

#include <vector>

class Space {
public:
    struct Polygon {
        unsigned int point_indexes[3];
        unsigned int colors_indexes[3];
        unsigned int texture_index;
        unsigned int tex_coords_indexes[3];
        unsigned int normal_indexes[3];
    };

public:
    Space();
    Space(unsigned int expected_size);

    void AddPoint(const Point4& point);
    void AddColor(uint32_t color);
    void AddTextureCoords(long double tex_x, long double tex_y);
    void AddTexture(const Image& texture);
    void AddNormal(const Vector4& normal);
    void AddPolygon(unsigned int point_index1, unsigned int point_index2,
                    unsigned int point_index3);

    void SetPolygonColors(unsigned int polygon_index, unsigned int color_index1,
                          unsigned int color_index2, unsigned int color_index3);
    void SetPolygonTextureCoords(unsigned int polygon_index, unsigned int tex_coords_index1,
                                 unsigned int tex_coords_index2, unsigned int tex_coords_index3);
    void SetPolygonTexture(unsigned int polygon_index, unsigned int texture_index);
    void SetPolygonNormals(unsigned int polygon_index, unsigned int normal_index1,
                           unsigned int normal_index2, unsigned int normal_index3);

    void SetPoint(unsigned int index, const Point4& point);
    Point4 GetPoint(unsigned int index) const;

    void SetColor(unsigned int index, uint32_t color);
    uint32_t GetColor(unsigned int index) const;

    void SetTexCoords(unsigned int index, long double tex_x, long double tex_y);
    std::pair<long double, long double> GetTexCoords(unsigned int index) const;

    void SetTexture(unsigned int index, const Image& texture);
    const Image* GetTexturePointer(unsigned int index) const;

    void SetNormal(unsigned int index, const Vector4& normal);
    Vector4 GetNormal(unsigned int index) const;

    Polygon GetPolygon(unsigned int index) const;

    void TransformPoint(unsigned int index, const Matrix4& transformation_matrix);
    void TransformNormal(unsigned int index, const Matrix4& transformation_matrix);

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
