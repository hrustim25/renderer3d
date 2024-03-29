#include "base_application.h"

class CubeApp : public rend::BaseApplication {
public:
    CubeApp() {
        camera_.Transform(rend::CreateMoveMatrix(0, 100, 100));
        camera_.Transform(rend::CreateRotationMatrix(2, 3.141592 / 4));
        transformation_matrix_ = rend::CreateRotationMatrix(0, 3.141592 / 240);

        rend::Point4 points[8];
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                for (int k = 0; k < 2; ++k) {
                    points[i * 4 + j * 2 + k](0) = i * 40 - 20;
                    points[i * 4 + j * 2 + k](1) = j * 40 - 20;
                    points[i * 4 + j * 2 + k](2) = k * 40 - 20;
                    points[i * 4 + j * 2 + k](3) = 1;
                }
            }
        }

        for (unsigned int i = 0; i < 8; ++i) {
            space_.AddPoint(points[i]);
        }
        for (unsigned int i = 0; i < 12; ++i) {
            uint32_t color = std::rand() % 256;
            color = (color << 8) + std::rand() % 256;
            color = (color << 8) + std::rand() % 256;
            color = (color << 8) + 255;
            space_.AddColor(color);
        }

        int points_to_poly[12][3] = {{0, 1, 2}, {1, 2, 3}, {0, 1, 4}, {1, 4, 5},
                                     {0, 2, 4}, {2, 4, 6}, {7, 6, 5}, {6, 5, 4},
                                     {7, 5, 3}, {5, 3, 1}, {7, 6, 3}, {6, 3, 2}};

        for (int i = 0; i < 12; ++i) {
            space_.AddPolygon(points_to_poly[i][0], points_to_poly[i][1], points_to_poly[i][2]);
            space_.SetPolygonColors(i, i, i, i);
        }
    }
};

int main() {
    CubeApp app;

    app.Start();

    return 0;
}
