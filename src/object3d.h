#pragma once

#include "primitives/pivot.h"

#include <vector>

class Object3d : public Pivot {
public:
    Object3d();
    Object3d(std::vector<int> vertex_indexes);

    void AddVertexIndex(int index);

    int GetVertexIndex(int position) const;

private:
    std::vector<int> vertex_indexes_;
};
