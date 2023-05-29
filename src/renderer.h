#pragma once

#include "camera.h"
#include "space.h"
#include "rasterizer.h"

namespace rend {

class Renderer {
public:
    Renderer() = default;
    Renderer(unsigned int screen_width, unsigned int screen_height);

    const Screen& GetScreen() const;

    const Screen& DrawScene(const Space& space, const Camera& camera);

private:
    bool IsPolygonVisible(const Point4& point1, const Point4& point2, const Point4& point3,
                          const Camera& camera) const;
    void DrawPolygon(const Vertex& vertex1, const Vertex& vertex2, const Vertex& vertex3,
                     const Camera& camera);

    Rasterizer rasterizer_;
};

}  // namespace rend
