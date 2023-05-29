#include "base_application.h"

class InterpolationApp : public rend::BaseApplication {
public:
    InterpolationApp() {
        camera_.Transform(rend::CreateMoveMatrix(100, -20, 0));
        camera_.Transform(rend::CreateRotationMatrix(1, 3.141592 / 2));
        transformation_matrix_ = rend::CreateRotationMatrix(1, 3.141592 / 240);

        rend::Point4 points[3];
        points[0] = {0, 50, 0, 1};
        points[1] = {30, 0, 0, 1};
        points[2] = {-30, 0, 0, 1};

        for (unsigned int i = 0; i < 3; ++i) {
            space_.AddPoint(points[i]);
        }

        rend::Vertex vert;
        vert.SetColor(255, 0, 0, 255);
        space_.AddColor(vert.GetColor());
        vert.SetColor(0, 255, 0, 255);
        space_.AddColor(vert.GetColor());
        vert.SetColor(0, 0, 255, 255);
        space_.AddColor(vert.GetColor());

        int points_to_poly[12][3] = {{0, 1, 2}, {1, 2, 3}, {0, 1, 4}, {1, 4, 5},
                                     {0, 2, 4}, {2, 4, 6}, {7, 6, 5}, {6, 5, 4},
                                     {7, 5, 3}, {5, 3, 1}, {7, 6, 3}, {6, 3, 2}};

        space_.AddPolygon(0, 1, 2);
        space_.SetPolygonColors(0, 0, 1, 2);
    }
};

int main() {
    InterpolationApp app;

    app.Start();

    return 0;
}
