#include "vertex.h"

namespace rend {

Vertex::Vertex(const Point4& point) : point_(point) {
}

Vertex::Vertex(const Point4& point, uint32_t color) : point_(point), color_(color) {
}

Vertex::Vertex(const Point4& point, std::pair<long double, long double> texture_coordinates,
               const Image* texture_ptr)
    : point_(point),
      tex_x_(texture_coordinates.first),
      tex_y_(texture_coordinates.second),
      texture_(texture_ptr) {
}

void Vertex::SetPoint(const Point4& point) {
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

void Vertex::SetColor(const Vector4& color_vector) {
    SetColor(color_vector(0) * 256, color_vector(1) * 256, color_vector(2) * 256,
             color_vector(3) * 256);
}

void Vertex::SetTextureCoordinates(long double tex_x, long double tex_y) {
    tex_x_ = tex_x;
    tex_y_ = tex_y;
}

void Vertex::SetTexturePointer(const Image* texture) {
    texture_ = texture;
}

long double AdjustBrightness(long double brightness) {
    return std::min(1.0L, std::max(0.0L, brightness));
}

void Vertex::SetNormal(const Vector4& normal) {
    Vector4 light_direction = {-1, 0, 0, 0};
    brightness_ = AdjustBrightness((Vector4::Transpose(normal) * light_direction)(0)) * 0.7 + 0.3;
}

const Point4& Vertex::GetPoint() const {
    return point_;
}

uint32_t Vertex::GetColor() const {
    return color_;
}

uint8_t Vertex::GetColorPart(unsigned int position) const {
    uint32_t result = color_;
    result <<= position * 8;
    result >>= 24;
    return result;
}

Vector4 Vertex::GetColorVector() const {
    return {static_cast<long double>(GetColorPart(0)) / 256,
            static_cast<long double>(GetColorPart(1)) / 256,
            static_cast<long double>(GetColorPart(2)) / 256,
            static_cast<long double>(GetColorPart(3)) / 256};
}

long double Vertex::GetTextureX() const {
    return tex_x_;
}

long double Vertex::GetTextureY() const {
    return tex_y_;
}

const Image* Vertex::GetTexturePointer() const {
    return texture_;
}

long double Vertex::GetBrightness() const {
    return brightness_;
}

}  // namespace rend
