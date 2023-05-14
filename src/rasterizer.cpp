#include "rasterizer.h"

#include <cmath>

Rasterizer::Rasterizer(unsigned screen_width, unsigned screen_height, const Camera &camera)
    : SCREEN_WIDTH_(screen_width), SCREEN_HEIGHT_(screen_height), camera_(camera) {
    color_buffer_ = std::make_unique<uint32_t[]>(screen_width * screen_height);
    zbuffer_ = std::make_unique<long double[]>(screen_width * screen_height);
}

void Rasterizer::SetFillMode(bool is_texture_mode) {
    is_texture_mode_ = is_texture_mode;
}

void Rasterizer::Clear() {
    for (unsigned i = 0; i < SCREEN_WIDTH_ * SCREEN_HEIGHT_; ++i) {
        color_buffer_[i] = 255;
        zbuffer_[i] = 1e18;
    }
}

Point4 NormalizeCoords(const Point4 &p) {
    return {p(0) / p(3), p(1) / p(3), p(2) / p(3), p(3)};
}

long double GetTriangleArea(const Vertex &vertex1, const Vertex &vertex2, const Vertex &vertex3) {
    Vector4 side1 = vertex2.GetPoint() - vertex1.GetPoint();
    Vector4 side2 = vertex3.GetPoint() - vertex1.GetPoint();
    return (side1(0) * side2(1) - side2(0) * side1(1)) / 2;
}

uint32_t ApplyBrightness(uint32_t color, long double brightness) {
    uint8_t r = (color >> 24) * brightness + 0.5;
    uint8_t g = ((color << 8) >> 24) * brightness + 0.5;
    uint8_t b = ((color << 16) >> 24) * brightness + 0.5;
    uint8_t a = (color << 24) >> 24;
    return (r << 24) + (g << 16) + (b << 8) + a;
}

long double NormalizeTextureCoordinate(long double coord) {
    coord -= (long long)coord;
    if (coord < 0) {
        coord += 1;
    }
    return coord;
}

void Rasterizer::FillRow(const Edge &left_edge, const Edge &right_edge, long long cur_y) {
    long long first_x = std::max(0.0L, std::ceil(left_edge.GetX()));
    long long last_x = std::min((long double)SCREEN_WIDTH_, std::ceil(right_edge.GetX()));
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

    long long point_position = cur_y * SCREEN_WIDTH_ + first_x;
    for (long long cur_x = first_x; cur_x < last_x; ++cur_x) {
        long double cur_z = 1.0L / cur_z_inv;
        if (cur_z < zbuffer_[point_position] && cur_z > 0) {
            zbuffer_[point_position] = cur_z;
            if (is_texture_mode_) {
                long long tex_x = NormalizeTextureCoordinate(cur_tex_x_over_z * cur_z) *
                                  left_edge.GetTexturePointer()->Width();
                long long tex_y = NormalizeTextureCoordinate(1 - cur_tex_y_over_z * cur_z) *
                                  left_edge.GetTexturePointer()->Height();
                color_buffer_[point_position] =
                    left_edge.GetTexturePointer()->GetPixel(tex_y, tex_x).GetColors();
            } else {
                cur_vertex.SetColor(cur_color);
                color_buffer_[point_position] = cur_vertex.GetColor();
            }
            color_buffer_[point_position] =
                ApplyBrightness(color_buffer_[point_position], cur_brightness);
        }

        ++point_position;
        cur_z_inv += z_inv_step;
        if (is_texture_mode_) {
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
    edge12.SetMode(is_texture_mode_);
    edge13.SetMode(is_texture_mode_);
    edge23.SetMode(is_texture_mode_);
    int type = GetTriangleArea(vertex1, vertex2, vertex3) >= 0;

    Edge left_edge = type ? edge13 : edge12;
    Edge right_edge = type ? edge12 : edge13;
    long long first_y = std::max(0.0L, std::ceil(vertex1.GetPoint()(1)));
    long long mid_y = std::min((long double)SCREEN_HEIGHT_, std::ceil(vertex2.GetPoint()(1)));
    long long last_y = std::min((long double)SCREEN_HEIGHT_, std::ceil(vertex3.GetPoint()(1)));

    left_edge.InitialStep(first_y);
    right_edge.InitialStep(first_y);
    for (long long i = first_y; i < mid_y; ++i) {
        FillRow(left_edge, right_edge, i);

        left_edge.Step();
        right_edge.Step();
    }

    mid_y = std::max(0.0L, std::ceil(vertex2.GetPoint()(1)));
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
    vs[0].SetPoint(NormalizeCoords(camera_.ProjectToScreen(vs[0].GetPoint())));
    vs[1].SetPoint(NormalizeCoords(camera_.ProjectToScreen(vs[1].GetPoint())));
    vs[2].SetPoint(NormalizeCoords(camera_.ProjectToScreen(vs[2].GetPoint())));
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

const uint32_t *Rasterizer::GetPixelArray() const {
    return color_buffer_.get();
}
