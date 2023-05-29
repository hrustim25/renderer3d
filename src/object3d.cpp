#include "object3d.h"

namespace rend {

Object3d::Object3d(std::vector<int> vertex_indexes) : vertex_indexes_(vertex_indexes) {
}

void Object3d::AddVertexIndex(int index) {
    vertex_indexes_.push_back(index);
}

int Object3d::GetVertexIndex(int position) const {
    return vertex_indexes_[position];
}

}  // namespace rend
