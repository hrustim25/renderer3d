#pragma once

#include "object3d.h"
#include "space.h"

namespace rend {

class ObjectReader {
public:
    ObjectReader() = default;

    Object3d ReadObject(const std::string& filename, Space& space) const;

private:
    Object3d ReadObjFile(const std::string& filename, Space& space) const;
};

}  // namespace rend
