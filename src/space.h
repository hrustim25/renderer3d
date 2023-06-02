#pragma once

#include "primitives/vertex.h"
#include "image.h"

#include <vector>

namespace rend {

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

    Vertex CreateVertex(unsigned int polygon_index, unsigned int vertex_index) const;

    void TransformAll(const Matrix4& transformation_matrix);

    size_t GetPointCount() const;
    size_t GetColorCount() const;
    size_t GetTexCoordsCount() const;
    size_t GetTextureCount() const;
    size_t GetNormalCount() const;
    size_t GetPolygonCount() const;

private:
    const Point4& GetPoint(unsigned int index) const;
    uint32_t GetColor(unsigned int index) const;
    std::pair<long double, long double> GetTexCoords(unsigned int index) const;
    const Image* GetTexturePointer(unsigned int index) const;
    const Vector4& GetNormal(unsigned int index) const;
    const Polygon& GetPolygon(unsigned int index) const;

    void SetPoint(unsigned int index, const Point4& point);
    void SetColor(unsigned int index, uint32_t color);
    void SetTexCoords(unsigned int index, long double tex_x, long double tex_y);
    void SetTexture(unsigned int index, const Image& texture);
    void SetNormal(unsigned int index, const Vector4& normal);

    std::vector<Point4> points_;
    std::vector<uint32_t> colors_;
    std::vector<std::pair<long double, long double>> tex_coords_;
    std::vector<Image> textures_;
    std::vector<Vector4> normals_;
    std::vector<Polygon> polygons_;
};

}  // namespace rend
