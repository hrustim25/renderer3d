#include "renderer.h"

int main() {
    rend::Renderer renderer;

    renderer.GetCamera().Transform(rend::CreateMoveMatrix(100, -20, 0));
    renderer.GetCamera().Transform(rend::CreateRotationMatrix(1, 3.141592 / 2));

    rend::Matrix4 rm = rend::CreateRotationMatrix(1, 3.141592 / 240);
    renderer.SetRotationMatrix(rm);

    rend::Point4 points[3];
    points[0] = {0, 50, 0, 1};
    points[1] = {30, 0, 0, 1};
    points[2] = {-30, 0, 0, 1};

    for (unsigned int i = 0; i < 3; ++i) {
        renderer.GetSpace().AddPoint(points[i]);
    }

    rend::Vertex vert;
    vert.SetColor(255, 0, 0, 255);
    renderer.GetSpace().AddColor(vert.GetColor());
    vert.SetColor(0, 255, 0, 255);
    renderer.GetSpace().AddColor(vert.GetColor());
    vert.SetColor(0, 0, 255, 255);
    renderer.GetSpace().AddColor(vert.GetColor());

    int points_to_poly[12][3] = {{0, 1, 2}, {1, 2, 3}, {0, 1, 4}, {1, 4, 5}, {0, 2, 4}, {2, 4, 6},
                                 {7, 6, 5}, {6, 5, 4}, {7, 5, 3}, {5, 3, 1}, {7, 6, 3}, {6, 3, 2}};

    renderer.GetSpace().AddPolygon(0, 1, 2);
    renderer.GetSpace().SetPolygonColors(0, 0, 1, 2);

    renderer.Start();

    return 0;
}
