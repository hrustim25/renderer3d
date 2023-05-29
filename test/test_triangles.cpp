#include "base_application.h"

class TrianglesApp : public rend::BaseApplication {
public:
    TrianglesApp() {
        camera_.Transform(rend::CreateMoveMatrix(0, 0, 200));
        camera_.Transform(rend::CreateRotationMatrix(0, 3.141592 / 2));
        transformation_matrix_ = rend::CreateRotationMatrix(2, 3.141592 / 240);

        rend::Point4 points[12];
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

        for (unsigned int i = 0; i < 12; ++i) {
            space_.AddPoint(points[i]);
        }
        for (unsigned int i = 0; i < 4; ++i) {
            uint32_t color = reds[i];
            color = (color << 8) + greens[i];
            color = (color << 8) + blues[i];
            color = (color << 8) + 255;
            space_.AddColor(color);
        }
        for (unsigned int i = 0; i < 4; ++i) {
            space_.AddPolygon(3 * i, 3 * i + 1, 3 * i + 2);
            space_.SetPolygonColors(i, i, i, i);
        }
    }
};

int main() {
    TrianglesApp app;

    app.Start();

    return 0;
}
