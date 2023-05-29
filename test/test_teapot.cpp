#include "object_reader.h"
#include "base_application.h"

class TeapotApp : public rend::BaseApplication {
public:
    TeapotApp() {
        camera_.Transform(rend::CreateMoveMatrix(0.5, 0, 0));
        camera_.Transform(rend::CreateRotationMatrix(1, 3.141592 / 2));
        transformation_matrix_ = rend::CreateRotationMatrix(1, 3.141592 / 240);
        rend::ObjectReader obj_reader;
        rend::Object3d obj = obj_reader.ReadObject("../models/utah-teapot.obj", space_);
    }
};

int main() {
    TeapotApp app;

    app.Start();

    return 0;
}
