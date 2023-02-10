#include "renderer.h"

Matrix3 BuildRotationMatrix() {
    Matrix3 result = CreateIdentityMatrix<3>();
    result *= CreateRotationMatrix(0, 3.141592 / 120);
    return result;
}

int main() {
    Renderer renderer;

    renderer.GetCamera().Move({0, -100, -100});
    renderer.GetCamera().Rotate(CreateRotationMatrix(2, 3.141592 / 4));

    Matrix3 rm = BuildRotationMatrix();
    renderer.SetRotationMatrix(rm);

    Point3 points[8];
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 2; ++k) {
                points[i * 4 + j * 2 + k] = Point3(i * 40 - 20, j * 40 - 20, k * 40 - 20);
            }
        }
    }

    for (unsigned i = 0; i < 8; ++i) {
        renderer.GetSpace().AddPoint(points[i]);
    }
    for (unsigned i = 0; i < 12; ++i) {
        uint32_t color = std::rand() % 256;
        color = (color << 8) + std::rand() % 256;
        color = (color << 8) + std::rand() % 256;
        color = (color << 8) + 255;
        renderer.GetSpace().AddColor(color);
    }

    int points_to_poly[12][3] = {{0, 1, 2}, {1, 2, 3}, {0, 1, 4}, {1, 4, 5}, {0, 2, 4}, {2, 4, 6},
                                 {7, 6, 5}, {6, 5, 4}, {7, 5, 3}, {5, 3, 1}, {7, 6, 3}, {6, 3, 2}};
    Vertex vs[36];
    for (int i = 0; i < 12; ++i) {
        renderer.GetSpace().AddPolygon(points_to_poly[i][0], points_to_poly[i][1],
                                       points_to_poly[i][2]);
        renderer.GetSpace().SetPolygonColors(i, i, i, i);
    }

    renderer.Start();

    return 0;
}
