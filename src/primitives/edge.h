#pragma once

#include "vertex.h"

namespace rend {

class Edge {
public:
    Edge(Vertex v_min, Vertex v_max);

    long double GetMinY() const;
    long double GetMaxY() const;
    long double GetX() const;
    long double GetZInv() const;
    Vector4 GetColor() const;
    long double GetTextureXOverZ() const;
    long double GetTextureYOverZ() const;
    const Image* GetTexturePointer() const;
    long double GetBrightness() const;

    void Next();

private:
    void Prestep(long long first_y);

    long double min_y_ = 0;
    long double max_y_ = 0;
    long double cur_x_ = 0;
    long double x_step_ = 0;
    long double cur_z_inv_ = 0;
    long double z_inv_step_ = 0;

    bool is_texture_mode_ = false;
    Vector4 cur_color_;
    Vector4 color_step_;
    long double cur_tex_x_over_z_ = 0;
    long double cur_tex_y_over_z_ = 0;
    long double tex_x_over_z_step_ = 0;
    long double tex_y_over_z_step_ = 0;
    const Image* texture_ = nullptr;
    long double cur_brightness_ = 0;
    long double brightness_step_ = 0;
};

}  // namespace rend
