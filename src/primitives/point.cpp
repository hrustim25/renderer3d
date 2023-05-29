#include "point.h"

namespace rend {

Point4 AdjustPointCoordinates(const Point4& point) {
    return {point(0) / point(3), point(1) / point(3), point(2) / point(3), 1};
}

}  // namespace rend
