#pragma once

#include "object3d.h"
#include "space.h"

namespace rend {

class ObjectReader {
public:
    static Object3d ReadObject(const std::string& filename, Space* space);

private:
    static Object3d ReadObjFile(const std::string& filename, Space* space);
};

}  // namespace rend
