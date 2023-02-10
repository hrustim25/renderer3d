#pragma once

#include "primitives/vertex.h"
#include "camera.h"

class Rasterizer {
public:
    Rasterizer(unsigned screen_width, unsigned screen_height, const Camera& camera);
    ~Rasterizer();

    void Clear();
    void DrawPolygon(const Vertex& vertex1, const Vertex& vertex2, const Vertex& vertex3);

    const uint32_t* GetPixelArray() const;

private:
    const unsigned SCREEN_WIDTH_;
    const unsigned SCREEN_HEIGHT_;
    uint32_t* color_buffer_;
    long double* zbuffer_;
    const Camera& camera_;
};
