#pragma once

#include "object3d.h"
#include "space.h"

class ObjectReader {
public:
    ObjectReader();

    Object3d ReadObject(const std::string& filename, Space& space) const;

private:
    Object3d ReadObjFile(const std::string& filename, Space& space) const;
};
