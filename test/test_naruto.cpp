#include "object_reader.h"
#include "base_application.h"

class NarutoApp : public rend::BaseApplication {
public:
    NarutoApp() {
        camera_.Transform(rend::CreateMoveMatrix(50, -13, 0));
        camera_.Transform(rend::CreateRotationMatrix(1, 3.141592 / 2));
        transformation_matrix_ = rend::CreateRotationMatrix(1, 3.141592 / 240);
        rend::ObjectReader obj_reader;
        rend::Object3d obj = obj_reader.ReadObject("../models/naruto.obj", space_);
    }
};

int main() {
    NarutoApp app;

    app.Start();

    return 0;
}
