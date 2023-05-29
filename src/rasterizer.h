#pragma once

#include "primitives/vertex.h"
#include "primitives/edge.h"
#include "screen.h"

namespace rend {

class Rasterizer {
public:
    Rasterizer() = default;
    Rasterizer(unsigned int screen_width, unsigned int screen_height);

    void DrawPolygon(const Vertex& vertex1, const Vertex& vertex2, const Vertex& vertex3);

    const Screen& GetScreen() const;

    void ClearScreen();

private:
    void FillRow(const Edge& left_edge, const Edge& right_edge, long long cur_y);

    void DrawOrientedTriangle(const Vertex& vertex1, const Vertex& vertex2, const Vertex& vertex3);

    Screen screen_;
};

}  // namespace rend
