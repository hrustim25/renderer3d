#include "rasterizer.h"

#include <cmath>

namespace rend {

Rasterizer::Rasterizer(unsigned int screen_width, unsigned int screen_height)
    : screen_(screen_width, screen_height) {
}

static bool GetTriangleType(const Vertex &vertex1, const Vertex &vertex2, const Vertex &vertex3) {
    Vector4 side1 = vertex2.GetPoint() - vertex1.GetPoint();
    Vector4 side2 = vertex3.GetPoint() - vertex1.GetPoint();
    return (side1(0) * side2(1) - side2(0) * side1(1)) >= 0;
}

void Rasterizer::FillRow(const Edge &left_edge, const Edge &right_edge, long long current_y) {
    RowIterator it(left_edge, right_edge);
    long long last_x = std::ceil(right_edge.GetX());
    if (last_x > screen_.GetWidth()) {
        last_x = screen_.GetWidth();
    }

    Vertex current_vertex;
    while (it.current_x < last_x) {
        long double current_z = 1.0L / it.current_z_inv;
        uint32_t pixel_color;
        if (left_edge.GetTexturePointer()) {
            long long tex_x =
                (it.current_tex_x_over_z * current_z) * left_edge.GetTexturePointer()->Width();
            long long tex_y =
                (1 - it.current_tex_y_over_z * current_z) * left_edge.GetTexturePointer()->Height();
            pixel_color = left_edge.GetTexturePointer()->GetPixel(tex_x, tex_y).GetColors();
        } else {
            current_vertex.SetColor(it.current_color);
            pixel_color = current_vertex.GetColor();
        }
        screen_.DrawPixel(it.current_x, current_y, current_z, pixel_color, it.current_brightness);

        it.Next();
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

    for (long long i = first_y; i < mid_y; ++i) {
        FillRow(left_edge, right_edge, i);

        left_edge.Next();
        right_edge.Next();
    }

    mid_y = std::ceil(vertex2.GetPoint()(1));
    if (mid_y < 0) {
        mid_y = 0;
    }
    if (type) {
        right_edge = edge23;
    } else {
        left_edge = edge23;
    }
    long long last_y = std::ceil(vertex3.GetPoint()(1));
    if (last_y > screen_.GetHeight()) {
        last_y = screen_.GetHeight();
    }

    for (long long i = mid_y; i < last_y; ++i) {
        FillRow(left_edge, right_edge, i);

        left_edge.Next();
        right_edge.Next();
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

Rasterizer::RowIterator::RowIterator(const Edge &left_edge, const Edge &right_edge) {
    current_x = std::ceil(left_edge.GetX());
    if (current_x < 0) {
        current_x = 0;
    }
    long double dist = right_edge.GetX() - left_edge.GetX();
    long double dx = current_x - left_edge.GetX();

    z_inv_step = (right_edge.GetZInv() - left_edge.GetZInv()) / dist;
    current_z_inv = left_edge.GetZInv() + z_inv_step * dx;
    color_step = (right_edge.GetColor() - left_edge.GetColor()) / dist;
    current_color = left_edge.GetColor() + color_step * dx;

    tex_x_over_z_step = (right_edge.GetTextureXOverZ() - left_edge.GetTextureXOverZ()) / dist;
    current_tex_x_over_z = left_edge.GetTextureXOverZ() + tex_x_over_z_step * dx;
    tex_y_over_z_step = (right_edge.GetTextureYOverZ() - left_edge.GetTextureYOverZ()) / dist;
    current_tex_y_over_z = left_edge.GetTextureYOverZ() + tex_y_over_z_step * dx;

    brightness_step = (right_edge.GetBrightness() - left_edge.GetBrightness()) / dist;
    current_brightness = left_edge.GetBrightness() + brightness_step * dx;
}

void Rasterizer::RowIterator::Next() {
    ++current_x;
    current_z_inv += z_inv_step;
    current_tex_x_over_z += tex_x_over_z_step;
    current_tex_y_over_z += tex_y_over_z_step;
    current_color += color_step;
    current_brightness += brightness_step;
}

}  // namespace rend
