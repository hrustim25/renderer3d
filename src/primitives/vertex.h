#pragma once

#include "point.h"
#include "../image.h"

class Vertex {
public:
    Vertex();
    Vertex(const Point4& point);
    Vertex(const Point4& point, uint32_t color);
    Vertex(const Point4& point, long double tex_x, long double tex_y);
    Vertex(const Point4& point, const Image* texture_ptr);

    void SetPoint(const Point4& point);
    void SetColor(uint32_t color);
    void SetColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
    void SetColor(const Vector4& color_vector);
    void SetTextureCoordinates(long double tex_x, long double tex_y);
    void SetTexturePointer(const Image* texture);
    void SetNormal(const Vector4& normal);
    void SetBrightness(long double brightness);

    Point4 GetPoint() const;
    uint32_t GetColor() const;
    uint8_t GetColorPart(unsigned int position) const;
    Vector4 GetColorVector() const;
    long double GetTextureX() const;
    long double GetTextureY() const;
    const Image* GetTexturePointer() const;
    Vector4 GetNormal() const;
    long double GetBrightness() const;

    static Vector4 GetLightDirection();

private:
    Point4 point_;
    uint32_t color_ = 0;  // r, g, b, a
    long double tex_x_ = 0;
    long double tex_y_ = 0;
    Vector4 normal_;
    long double brightness_ = 1;

    const Image* texture_ = nullptr;

    static Vector4 light_direction_;
};
