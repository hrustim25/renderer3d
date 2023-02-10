#include "vertex.h"

Vertex::Vertex() : color_(255) {
}

Vertex::Vertex(const Point3& point) : point_(point), color_(255) {
}

Vertex::Vertex(const Point3& point, uint32_t color) : point_(point), color_(color) {
}

void Vertex::SetPoint(const Point3& point) {
    point_ = point;
}

void Vertex::SetColor(uint32_t color) {
    color_ = color;
}

void Vertex::SetColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
    uint32_t result_color = red;
    result_color = (result_color << 8) + green;
    result_color = (result_color << 8) + blue;
    result_color = (result_color << 8) + alpha;
    color_ = result_color;
}

Point3 Vertex::GetPoint() const {
    return point_;
}

uint32_t Vertex::GetColor() const {
    return color_;
}

uint8_t Vertex::GetColorPart(unsigned position) const {
    uint32_t result = color_;
    result <<= position * 8;
    result >>= 24;
    return result;
}
