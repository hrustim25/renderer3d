#include "renderer.h"

#include "object_reader.h"

int main() {
    rend::Renderer renderer;

    renderer.GetCamera().Transform(rend::CreateMoveMatrix(50, -13, 0));
    renderer.GetCamera().Transform(rend::CreateRotationMatrix(1, 3.141592 / 2));

    rend::Matrix4 rm = rend::CreateRotationMatrix(1, 3.141592 / 240);
    renderer.SetRotationMatrix(rm);

    rend::ObjectReader obj_reader;
    rend::Object3d obj = obj_reader.ReadObject("../models/naruto.obj", renderer.GetSpace());

    renderer.SetFillMode(true);

    for (unsigned int i = 0; i < renderer.GetSpace().GetPolygonCount(); ++i) {
        renderer.GetSpace().SetPolygonTexture(i, renderer.GetSpace().GetPolygon(i).texture_index);
    }

    renderer.Start();

    return 0;
}
