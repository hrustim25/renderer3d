#include "edge.h"

#include <cmath>

namespace rend {

Edge::Edge(Vertex v_min, Vertex v_max)
    : min_y_(v_min.GetPoint()(1)),
      max_y_(v_max.GetPoint()(1)),
      is_texture_mode_(v_min.GetTexturePointer() != nullptr) {
    long double dy = max_y_ - min_y_;

    cur_x_ = v_min.GetPoint()(0);
    x_step_ = (v_max.GetPoint()(0) - v_min.GetPoint()(0)) / dy;
    cur_z_inv_ = 1.0 / v_min.GetPoint()(2);
    z_inv_step_ = (1.0 / v_max.GetPoint()(2) - 1.0 / v_min.GetPoint()(2)) / dy;
    cur_color_ = v_min.GetColorVector();
    color_step_ = (v_max.GetColorVector() - v_min.GetColorVector()) / dy;

    cur_tex_x_over_z_ = v_min.GetTextureX() / v_min.GetPoint()(2);
    cur_tex_y_over_z_ = v_min.GetTextureY() / v_min.GetPoint()(2);
    tex_x_over_z_step_ =
        (v_max.GetTextureX() / v_max.GetPoint()(2) - v_min.GetTextureX() / v_min.GetPoint()(2)) /
        dy;
    tex_y_over_z_step_ =
        (v_max.GetTextureY() / v_max.GetPoint()(2) - v_min.GetTextureY() / v_min.GetPoint()(2)) /
        dy;
    texture_ = v_min.GetTexturePointer();

    cur_brightness_ = v_min.GetBrightness();
    brightness_step_ = (v_max.GetBrightness() - v_min.GetBrightness()) / dy;

    long long first_y = std::ceil(v_min.GetPoint()(1));
    if (first_y < 0) {
        first_y = 0;
    }
    Prestep(first_y);
}

long double Edge::GetMinY() const {
    return min_y_;
}

long double Edge::GetMaxY() const {
    return max_y_;
}

long double Edge::GetX() const {
    return cur_x_;
}

long double Edge::GetZInv() const {
    return cur_z_inv_;
}

Vector4 Edge::GetColor() const {
    return cur_color_;
}

long double Edge::GetTextureXOverZ() const {
    return cur_tex_x_over_z_;
}

long double Edge::GetTextureYOverZ() const {
    return cur_tex_y_over_z_;
}

const Image* Edge::GetTexturePointer() const {
    return texture_;
}

long double Edge::GetBrightness() const {
    return cur_brightness_;
}

void Edge::Prestep(long long first_y) {
    long double dy = first_y - min_y_;

    min_y_ = first_y;
    cur_x_ += x_step_ * dy;
    cur_z_inv_ += z_inv_step_ * dy;
    if (is_texture_mode_) {
        cur_tex_x_over_z_ += tex_x_over_z_step_ * dy;
        cur_tex_y_over_z_ += tex_y_over_z_step_ * dy;
    } else {
        cur_color_ += color_step_ * dy;
    }
    cur_brightness_ += brightness_step_ * dy;
}

void Edge::Next() {
    cur_x_ += x_step_;
    cur_z_inv_ += z_inv_step_;
    if (is_texture_mode_) {
        cur_tex_x_over_z_ += tex_x_over_z_step_;
        cur_tex_y_over_z_ += tex_y_over_z_step_;
    } else {
        cur_color_ += color_step_;
    }
    cur_brightness_ += brightness_step_;
}

}  // namespace rend
