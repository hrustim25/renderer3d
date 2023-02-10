#include "renderer.h"

Matrix3 BuildRotationMatrix() {
    Matrix3 result = CreateIdentityMatrix<3>();
    result *= CreateRotationMatrix(2, 3.141592 / 120);
    return result;
}

int main() {
    Renderer renderer;

    renderer.GetCamera().Move({0, 0, -200});
    renderer.GetCamera().Rotate(CreateRotationMatrix(0, 3.141592 / 2));

    Matrix3 rm = BuildRotationMatrix();
    renderer.SetRotationMatrix(rm);

    Point3 points[12];
    points[0] = Point3(50, 50, 20);
    points[1] = Point3(50, 20, 20);
    points[2] = Point3(-70, 20, -20);

    points[3] = Point3(-50, 50, 20);
    points[4] = Point3(-20, 50, 20);
    points[5] = Point3(-20, -70, -20);

    points[6] = Point3(-50, -50, 20);
    points[7] = Point3(-50, -20, 20);
    points[8] = Point3(70, -20, -20);

    points[9] = Point3(50, -50, 20);
    points[10] = Point3(20, -50, 20);
    points[11] = Point3(20, 70, -20);

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
