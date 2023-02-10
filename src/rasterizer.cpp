#include "rasterizer.h"

#include <cmath>

Rasterizer::Rasterizer(unsigned screen_width, unsigned screen_height, const Camera& camera)
    : SCREEN_WIDTH_(screen_width), SCREEN_HEIGHT_(screen_height), camera_(camera) {
    color_buffer_ = new uint32_t[screen_width * screen_height];
    zbuffer_ = new long double[screen_width * screen_height];
}

Rasterizer::~Rasterizer() {
    delete[] color_buffer_;
    delete[] zbuffer_;
}

void Rasterizer::Clear() {
    for (unsigned i = 0; i < SCREEN_WIDTH_ * SCREEN_HEIGHT_; ++i) {
        color_buffer_[i] = 255;
        zbuffer_[i] = 1e18;
    }
}

void Rasterizer::DrawPolygon(const Vertex& vertex1, const Vertex& vertex2, const Vertex& vertex3) {
    Vertex vs[3];
    vs[0] = vertex1;
    vs[1] = vertex2;
    vs[2] = vertex3;
    vs[0].SetPoint(camera_.ProjectToScreen(vs[0].GetPoint()));
    vs[1].SetPoint(camera_.ProjectToScreen(vs[1].GetPoint()));
    vs[2].SetPoint(camera_.ProjectToScreen(vs[2].GetPoint()));
    if (vs[0].GetPoint()(0) > vs[1].GetPoint()(0)) {
        std::swap(vs[0], vs[1]);
    }
    if (vs[0].GetPoint()(0) > vs[2].GetPoint()(0)) {
        std::swap(vs[0], vs[2]);
    }
    if (vs[1].GetPoint()(0) > vs[2].GetPoint()(0)) {
        std::swap(vs[1], vs[2]);
    }
    long double minor1 =
        (vs[1].GetPoint()(1) - vs[0].GetPoint()(1)) * (vs[2].GetPoint()(2) - vs[0].GetPoint()(2)) -
        (vs[1].GetPoint()(2) - vs[0].GetPoint()(2)) * (vs[2].GetPoint()(1) - vs[0].GetPoint()(1));
    long double minor2 =
        (vs[1].GetPoint()(0) - vs[0].GetPoint()(0)) * (vs[2].GetPoint()(2) - vs[0].GetPoint()(2)) -
        (vs[1].GetPoint()(2) - vs[0].GetPoint()(2)) * (vs[2].GetPoint()(0) - vs[0].GetPoint()(0));
    long double minor3 =
        (vs[1].GetPoint()(0) - vs[0].GetPoint()(0)) * (vs[2].GetPoint()(1) - vs[0].GetPoint()(1)) -
        (vs[1].GetPoint()(1) - vs[0].GetPoint()(1)) * (vs[2].GetPoint()(0) - vs[0].GetPoint()(0));
    long double dx01 = vs[1].GetPoint()(0) - vs[0].GetPoint()(0);
    long double dx02 = vs[2].GetPoint()(0) - vs[0].GetPoint()(0);
    long double dy01 = vs[1].GetPoint()(1) - vs[0].GetPoint()(1);
    long double dy02 = vs[2].GetPoint()(1) - vs[0].GetPoint()(1);
    Vector3 difference_vector1 = vertex2.GetPoint().ToVector() - vertex1.GetPoint().ToVector();
    Vector3 difference_vector2 = vertex3.GetPoint().ToVector() - vertex1.GetPoint().ToVector();
    long double y_lower_bound = vs[0].GetPoint()(1);
    long double y_upper_bound = vs[0].GetPoint()(1);
    if (dx01 >= 0.1) {
        long double y_step_lower = dy01 / dx01;
        long double y_step_upper = dy02 / dx02;
        if (y_step_lower > y_step_upper) {
            std::swap(y_step_lower, y_step_upper);
        }
        long long x_first = std::max(.0l, vs[0].GetPoint()(0));
        long long x_last = std::min((long double)SCREEN_WIDTH_, vs[1].GetPoint()(0));
        for (long long i = x_first; i < x_last; ++i) {
            long long y_first = std::max(.0l, std::floor(y_lower_bound));
            long long y_last = std::min((long double)SCREEN_HEIGHT_ - 1, std::floor(y_upper_bound));
            for (long long j = y_first; j <= y_last; ++j) {
                long double current_z = vs[0].GetPoint()(2) + ((j - vs[0].GetPoint()(1)) * minor2 -
                                                               (i - vs[0].GetPoint()(0)) * minor1) /
                                                                  minor3;
                long long point_position = j * SCREEN_WIDTH_ + i;
                if (current_z < zbuffer_[point_position]) {
                    color_buffer_[point_position] = vs[0].GetColor();
                    zbuffer_[point_position] = current_z;
                }
            }
            y_lower_bound += y_step_lower;
            y_upper_bound += y_step_upper;
        }
    }
    long double dx12 = vs[2].GetPoint()(0) - vs[1].GetPoint()(0);
    long double dy12 = vs[2].GetPoint()(1) - vs[1].GetPoint()(1);
    if (dx12 >= 0.1) {
        y_lower_bound = (dy02 / dx02) * dx01 + vs[0].GetPoint()(1);
        y_upper_bound = vs[1].GetPoint()(1);
        if (y_lower_bound > y_upper_bound) {
            std::swap(y_lower_bound, y_upper_bound);
        }
        long double y_step_lower = dy02 / dx02;
        long double y_step_upper = dy12 / dx12;
        if (y_step_lower < y_step_upper) {
            std::swap(y_step_lower, y_step_upper);
        }
        long long x_first = std::max(.0l, vs[1].GetPoint()(0));
        long long x_last = std::min((long double)SCREEN_WIDTH_, vs[2].GetPoint()(0));
        for (long long i = x_first; i < x_last; ++i) {
            long long y_first = std::max(.0l, std::floor(y_lower_bound));
            long long y_last = std::min((long double)SCREEN_HEIGHT_ - 1, std::floor(y_upper_bound));
            for (long long j = y_first; j <= y_last; ++j) {
                long double current_z = vs[0].GetPoint()(2) + ((j - vs[0].GetPoint()(1)) * minor2 -
                                                               (i - vs[0].GetPoint()(0)) * minor1) /
                                                                  minor3;
                long long point_position = j * SCREEN_WIDTH_ + i;
                if (current_z < zbuffer_[point_position]) {
                    color_buffer_[point_position] = vs[0].GetColor();
                    zbuffer_[point_position] = current_z;
                }
            }
            y_lower_bound += y_step_lower;
            y_upper_bound += y_step_upper;
        }
    }
}

const uint32_t* Rasterizer::GetPixelArray() const {
    return color_buffer_;
}
