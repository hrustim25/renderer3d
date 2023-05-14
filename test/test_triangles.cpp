#include "renderer.h"

int main() {
    Renderer renderer;

    renderer.GetCamera().Transform(CreateMoveMatrix(0, 0, 200));
    renderer.GetCamera().Transform(CreateRotationMatrix(0, 3.141592 / 2));

    Matrix4 rm = CreateRotationMatrix(2, 3.141592 / 240);
    renderer.SetRotationMatrix(rm);

    Point4 points[12];
    points[0] = {50, 50, 20, 1};
    points[1] = {50, 20, 20, 1};
    points[2] = {-70, 20, -20, 1};

    points[3] = {-50, 50, 20, 1};
    points[4] = {-20, 50, 20, 1};
    points[5] = {-20, -70, -20, 1};

    points[6] = {-50, -50, 20, 1};
    points[7] = {-50, -20, 20, 1};
    points[8] = {70, -20, -20, 1};

    points[9] = {50, -50, 20, 1};
    points[10] = {20, -50, 20, 1};
    points[11] = {20, 70, -20, 1};

    uint8_t reds[4] = {255, 220, 0, 0};
    uint8_t greens[4] = {0, 220, 255, 0};
    uint8_t blues[4] = {0, 220, 0, 255};

    for (unsigned i = 0; i < 12; ++i) {
        renderer.GetSpace().AddPoint(points[i]);
    }
    for (unsigned i = 0; i < 4; ++i) {
        uint32_t color = reds[i];
        color = (color << 8) + greens[i];
        color = (color << 8) + blues[i];
        color = (color << 8) + 255;
        renderer.GetSpace().AddColor(color);
    }
    for (unsigned i = 0; i < 4; ++i) {
        renderer.GetSpace().AddPolygon(3 * i, 3 * i + 1, 3 * i + 2);
        renderer.GetSpace().SetPolygonColors(i, i, i, i);
    }

    renderer.Start();

    return 0;
}
