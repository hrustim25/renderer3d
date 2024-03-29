#pragma once

#include "primitives/pivot.h"

#include <vector>

namespace rend {

class Object3d : public Pivot {
public:
    Object3d() = default;
    Object3d(std::vector<int> vertex_indexes);

    void AddVertexIndex(int index);

    int GetVertexIndex(int position) const;

private:
    std::vector<int> vertex_indexes_;
};

}  // namespace rend
