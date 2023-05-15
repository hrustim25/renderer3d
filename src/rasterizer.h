#pragma once

#include "primitives/vertex.h"
#include "primitives/edge.h"
#include "camera.h"

#include <memory>

class Rasterizer {
public:
    Rasterizer(unsigned screen_width, unsigned screen_height, const Camera& camera);

    void SetFillMode(bool is_texture_mode);

    void Clear();
    void DrawPolygon(const Vertex& vertex1, const Vertex& vertex2, const Vertex& vertex3);

    const uint32_t* GetPixelArray() const;

private:
    void FillRow(const Edge& left_edge, const Edge& right_edge, long long cur_y);

    void DrawOrientedTriangle(const Vertex& vertex1, const Vertex& vertex2, const Vertex& vertex3);

    bool IsTriangleVisible(const Point4& point1, const Point4& point2, const Point4& point3) const;

    const unsigned SCREEN_WIDTH_;
    const unsigned SCREEN_HEIGHT_;
    std::unique_ptr<uint32_t[]> color_buffer_;
    std::unique_ptr<long double[]> zbuffer_;
    const Camera& camera_;

    bool is_texture_mode_ = false;
};
