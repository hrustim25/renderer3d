#pragma once

#include "camera.h"
#include "space.h"
#include "rasterizer.h"

class Renderer {
public:
    Renderer();
    Renderer(unsigned screen_width, unsigned screen_height);

    Camera& GetCamera();
    Space& GetSpace();

    void SetRotationMatrix(const Matrix3& matrix);

    void Start();

private:
    const unsigned SCREEN_WIDTH_ = 800;
    const unsigned SCREEN_HEIGHT_ = 600;

    Camera camera_;
    Space space_;
    Rasterizer rasterizer_;

    Matrix3 rotation_matrix_;
};
