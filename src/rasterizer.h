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
    struct RowIterator {
        RowIterator(const Edge& left_edge, const Edge& right_edge);

        void Next();

        long long current_x;
        long double z_inv_step;
        long double current_z_inv;
        Vector4 color_step;
        Vector4 current_color;
        long double tex_x_over_z_step;
        long double current_tex_x_over_z;
        long double tex_y_over_z_step;
        long double current_tex_y_over_z;
        long double brightness_step;
        long double current_brightness;
    };

    void FillRow(const Edge& left_edge, const Edge& right_edge, long long current_y);

    void DrawOrientedTriangle(const Vertex& vertex1, const Vertex& vertex2, const Vertex& vertex3);

    Screen screen_;
};

}  // namespace rend
