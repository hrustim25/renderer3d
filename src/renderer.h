#pragma once

#include "camera.h"
#include "space.h"
#include "rasterizer.h"

namespace rend {

class Renderer {
public:
    Renderer();

    void SetFillMode(bool is_texture_mode);

    Camera& GetCamera();
    Space& GetSpace();

    void SetRotationMatrix(const Matrix4& matrix);

    void Start();

private:
    static constexpr unsigned int k_screen_width = 800;
    static constexpr unsigned int k_screen_height = 600;

    bool is_texture_mode_ = false;

    Camera camera_;
    Space space_;
    Rasterizer rasterizer_;

    Matrix4 transformation_matrix_;
};

}  // namespace rend
