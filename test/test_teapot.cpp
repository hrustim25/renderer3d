#include "renderer.h"

#include "object_reader.h"

int main() {
    Renderer renderer;

    renderer.GetCamera().Transform(CreateMoveMatrix(0.5, 0, 0));
    renderer.GetCamera().Transform(CreateRotationMatrix(1, 3.141592 / 2));

    Matrix4 rm = CreateRotationMatrix(1, 3.141592 / 240);
    renderer.SetRotationMatrix(rm);

    ObjectReader obj_reader;
    Object3d obj = obj_reader.ReadObject("../models/utah-teapot.obj", renderer.GetSpace());

    renderer.SetFillMode(true);

    for (unsigned i = 0; i < renderer.GetSpace().GetPolygonCount(); ++i) {
        renderer.GetSpace().SetPolygonTexture(i, renderer.GetSpace().GetPolygon(i).texture_index);
    }

    renderer.Start();

    return 0;
}
