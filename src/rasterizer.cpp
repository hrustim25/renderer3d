#include "rasterizer.h"

#include <cmath>

namespace rend {

Rasterizer::Rasterizer(unsigned int screen_width, unsigned int screen_height)
    : screen_(screen_width, screen_height) {
}

bool GetTriangleType(const Vertex &vertex1, const Vertex &vertex2, const Vertex &vertex3) {
    Vector4 side1 = vertex2.GetPoint() - vertex1.GetPoint();
    Vector4 side2 = vertex3.GetPoint() - vertex1.GetPoint();
    return (side1(0) * side2(1) - side2(0) * side1(1)) >= 0;
}

void Rasterizer::FillRow(const Edge &left_edge, const Edge &right_edge, long long cur_y) {
    long long first_x = std::ceil(left_edge.GetX());
    if (first_x < 0) {
        first_x = 0;
    }
    long long last_x = std::ceil(right_edge.GetX());
    if (last_x > screen_.GetWidth()) {
        last_x = screen_.GetWidth();
    }
    long double dist = right_edge.GetX() - left_edge.GetX();
    long double dx = first_x - left_edge.GetX();

    long double z_inv_step = (right_edge.GetZInv() - left_edge.GetZInv()) / dist;
    long double cur_z_inv = left_edge.GetZInv() + z_inv_step * dx;
    Vector4 color_step = (right_edge.GetColor() - left_edge.GetColor()) / dist;
    Vector4 cur_color = left_edge.GetColor() + color_step * dx;

    long double tex_x_over_z_step =
        (right_edge.GetTextureXOverZ() - left_edge.GetTextureXOverZ()) / dist;
    long double cur_tex_x_over_z = left_edge.GetTextureXOverZ() + tex_x_over_z_step * dx;
    long double tex_y_over_z_step =
        (right_edge.GetTextureYOverZ() - left_edge.GetTextureYOverZ()) / dist;
    long double cur_tex_y_over_z = left_edge.GetTextureYOverZ() + tex_y_over_z_step * dx;

    long double brightness_step = (right_edge.GetBrightness() - left_edge.GetBrightness()) / dist;
    long double cur_brightness = left_edge.GetBrightness() + brightness_step * dx;

    Vertex cur_vertex;

    for (long long cur_x = first_x; cur_x < last_x; ++cur_x) {
        long double cur_z = 1.0L / cur_z_inv;
        uint32_t pixel_color;
        if (left_edge.GetTexturePointer()) {
            long long tex_x = (cur_tex_x_over_z * cur_z) * left_edge.GetTexturePointer()->Width();
            long long tex_y =
                (1 - cur_tex_y_over_z * cur_z) * left_edge.GetTexturePointer()->Height();
            pixel_color = left_edge.GetTexturePointer()->GetPixel(tex_x, tex_y).GetColors();
        } else {
            cur_vertex.SetColor(cur_color);
            pixel_color = cur_vertex.GetColor();
        }
        screen_.DrawPixel(cur_x, cur_y, cur_z, pixel_color, cur_brightness);

        cur_z_inv += z_inv_step;
        if (left_edge.GetTexturePointer()) {
            cur_tex_x_over_z += tex_x_over_z_step;
            cur_tex_y_over_z += tex_y_over_z_step;
        } else {
            cur_color += color_step;
        }
        cur_brightness += brightness_step;
    }
}

void Rasterizer::DrawOrientedTriangle(const Vertex &vertex1, const Vertex &vertex2,
                                      const Vertex &vertex3) {
    Edge edge12(vertex1, vertex2);
    Edge edge13(vertex1, vertex3);
    Edge edge23(vertex2, vertex3);
    bool type = GetTriangleType(vertex1, vertex2, vertex3);

    Edge left_edge = type ? edge13 : edge12;
    Edge right_edge = type ? edge12 : edge13;
    long long first_y = std::ceil(vertex1.GetPoint()(1));
    if (first_y < 0) {
        first_y = 0;
    }
    long long mid_y = std::ceil(vertex2.GetPoint()(1));
    if (mid_y > screen_.GetHeight()) {
        mid_y = screen_.GetHeight();
    }
    long long last_y = std::ceil(vertex3.GetPoint()(1));
    if (last_y > screen_.GetHeight()) {
        last_y = screen_.GetHeight();
    }

    left_edge.InitialStep(first_y);
    right_edge.InitialStep(first_y);
    for (long long i = first_y; i < mid_y; ++i) {
        FillRow(left_edge, right_edge, i);

        left_edge.Step();
        right_edge.Step();
    }

    mid_y = std::ceil(vertex2.GetPoint()(1));
    if (mid_y < 0) {
        mid_y = 0;
    }
    edge23.InitialStep(mid_y);
    if (type) {
        right_edge = edge23;
    } else {
        left_edge = edge23;
    }

    for (long long i = mid_y; i < last_y; ++i) {
        FillRow(left_edge, right_edge, i);

        left_edge.Step();
        right_edge.Step();
    }
}

void Rasterizer::DrawPolygon(const Vertex &vertex1, const Vertex &vertex2, const Vertex &vertex3) {
    Vertex vs[3];
    vs[0] = vertex1;
    vs[1] = vertex2;
    vs[2] = vertex3;
    if (vs[0].GetPoint()(1) > vs[1].GetPoint()(1)) {
        std::swap(vs[0], vs[1]);
    }
    if (vs[0].GetPoint()(1) > vs[2].GetPoint()(1)) {
        std::swap(vs[0], vs[2]);
    }
    if (vs[1].GetPoint()(1) > vs[2].GetPoint()(1)) {
        std::swap(vs[1], vs[2]);
    }
    DrawOrientedTriangle(vs[0], vs[1], vs[2]);
}

const Screen &Rasterizer::GetScreen() const {
    return screen_;
}

void Rasterizer::ClearScreen() {
    screen_.Clear();
}

}  // namespace rend
