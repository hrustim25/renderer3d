#include "renderer.h"

namespace rend {

Renderer::Renderer(unsigned int screen_width, unsigned int screen_height)
    : rasterizer_(screen_width, screen_height) {
}

const Screen& Renderer::GetScreen() const {
    return rasterizer_.GetScreen();
}

const Screen& Renderer::DrawScene(const Space& space, const Camera& camera) {
    rasterizer_.ClearScreen();
    for (size_t i = 0; i < space.GetPolygonCount(); ++i) {
        const Space::Polygon& polygon = space.GetPolygon(i);
        Vertex vs[3];
        for (size_t j = 0; j < 3; ++j) {
            if (polygon.texture_index) {
                vs[j] = Vertex(space.GetPoint(polygon.point_indexes[j]),
                               space.GetTexCoords(polygon.tex_coords_indexes[j]),
                               space.GetTexturePointer(polygon.texture_index));
            } else {
                vs[j] = Vertex(space.GetPoint(polygon.point_indexes[j]),
                               space.GetColor(polygon.colors_indexes[j]));
            }
            vs[j].SetNormal(space.GetNormal(polygon.normal_indexes[j]));
        }
        DrawPolygon(vs[0], vs[1], vs[2], camera);
    }
    return rasterizer_.GetScreen();
}

bool Renderer::IsPolygonVisible(const Point4& point1, const Point4& point2, const Point4& point3,
                                const Camera& camera) const {
    if (point1(2) < camera.GetNearClipDistance() && point2(2) < camera.GetNearClipDistance() &&
        point3(2) < camera.GetNearClipDistance()) {
        return false;
    }
    if (point1(2) > camera.GetClipDistance() && point2(2) > camera.GetClipDistance() &&
        point3(2) > camera.GetClipDistance()) {
        return false;
    }
    if (point1(0) < -camera.GetViewPiramidWidthTan() * point1(2) &&
        point2(0) < -camera.GetViewPiramidWidthTan() * point2(2) &&
        point3(0) < -camera.GetViewPiramidWidthTan() * point3(2)) {
        return false;
    }
    if (point1(0) > camera.GetViewPiramidWidthTan() * point1(2) &&
        point2(0) > camera.GetViewPiramidWidthTan() * point2(2) &&
        point3(0) > camera.GetViewPiramidWidthTan() * point3(2)) {
        return false;
    }
    if (point1(1) < -camera.GetViewPiramidHeightTan() * point1(2) &&
        point2(1) < -camera.GetViewPiramidHeightTan() * point2(2) &&
        point3(1) < -camera.GetViewPiramidHeightTan() * point3(2)) {
        return false;
    }
    if (point1(1) > camera.GetViewPiramidHeightTan() * point1(2) &&
        point2(1) > camera.GetViewPiramidHeightTan() * point2(2) &&
        point3(1) > camera.GetViewPiramidHeightTan() * point3(2)) {
        return false;
    }
    return true;
}

void Renderer::DrawPolygon(const Vertex& vertex1, const Vertex& vertex2, const Vertex& vertex3,
                           const Camera& camera) {
    Vertex vs[3];
    vs[0] = vertex1;
    vs[1] = vertex2;
    vs[2] = vertex3;
    Point4 point1 = camera.ToLocalCoordinates(vs[0].GetPoint());
    Point4 point2 = camera.ToLocalCoordinates(vs[1].GetPoint());
    Point4 point3 = camera.ToLocalCoordinates(vs[2].GetPoint());

    if (!IsPolygonVisible(point1, point2, point3, camera)) {
        return;
    }

    vs[0].SetPoint(AdjustPointCoordinates(camera.ProjectToScreen(point1)));
    vs[1].SetPoint(AdjustPointCoordinates(camera.ProjectToScreen(point2)));
    vs[2].SetPoint(AdjustPointCoordinates(camera.ProjectToScreen(point3)));

    rasterizer_.DrawPolygon(vs[0], vs[1], vs[2]);
}

}  // namespace rend
