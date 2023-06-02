#include <cstdio>
#include <stdexcept>

#include <SFML/Graphics.hpp>

#include "image.h"

namespace rend {

uint32_t RGB::GetColors() const {
    uint32_t result = r;
    result = (result << 8) + g;
    result = (result << 8) + b;
    result = (result << 8) + UINT8_MAX;
    return result;
}

Image::Image(size_t width, size_t height) {
    SetSize(width, height);
}

Image::Image(const std::string &filename) {
    ReadImage(filename);
}

void Image::SetSize(size_t width, size_t height) {
    width_ = width;
    height_ = height;
    data_.assign(width * height, RGB());
}

size_t Image::Width() const {
    return width_;
}

size_t Image::Height() const {
    return height_;
}

size_t Image::GetActualArrayPosition(int x, int y) const {
    x = x % Width();
    if (x < 0) {
        x += Width();
    }
    y = y % Height();
    if (y < 0) {
        y += Height();
    }
    return x * height_ + y;
}

RGB Image::GetPixel(int x, int y) const {
    return data_[GetActualArrayPosition(x, y)];
}

void Image::SetPixel(int x, int y, const RGB &pixel) {
    data_[GetActualArrayPosition(x, y)] = pixel;
}

RGB &Image::Pixel(int x, int y) {
    return data_[GetActualArrayPosition(x, y)];
}

void Image::ReadImage(const std::string &filename) {
    sf::Image img;
    bool is_load_successful = img.loadFromFile("../models/" + filename);
    if (!is_load_successful) {
        return;
    }
    sf::Vector2u img_size = img.getSize();
    SetSize(img_size.x, img_size.y);
    for (int x = 0; x < Width(); ++x) {
        for (int y = 0; y < Height(); ++y) {
            sf::Color pixel_color = img.getPixel(x, y);
            SetPixel(x, y, RGB{pixel_color.r, pixel_color.g, pixel_color.b});
        }
    }
}

}  // namespace rend
