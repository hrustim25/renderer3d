#pragma once

#include "camera.h"
#include "space.h"
#include "rasterizer.h"

class Renderer {
public:
    Renderer();
    Renderer(unsigned screen_width, unsigned screen_height);

    void SetFillMode(bool is_texture_mode);

    Camera& GetCamera();
    Space& GetSpace();

    void SetRotationMatrix(const Matrix4& matrix);

    void Start();

private:
    const unsigned SCREEN_WIDTH_ = 800;
    const unsigned SCREEN_HEIGHT_ = 600;

    bool is_texture_mode_ = false;

    Camera camera_;
    Space space_;
    Rasterizer rasterizer_;

    Matrix4 transformation_matrix_;
};
