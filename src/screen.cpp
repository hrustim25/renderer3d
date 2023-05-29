#include "screen.h"

namespace rend {

static unsigned int default_color_value = 255;
static long double default_z_value = 1e18;

Screen::Screen() {
    color_buffer_.assign(k_screen_width_ * k_screen_height_, default_color_value);
    zbuffer_.assign(k_screen_width_ * k_screen_height_, default_z_value);
}

Screen::Screen(unsigned int screen_width, unsigned int screen_height)
    : k_screen_width_(screen_width), k_screen_height_(screen_height) {
    color_buffer_.assign(k_screen_width_ * k_screen_height_, default_color_value);
    zbuffer_.assign(k_screen_width_ * k_screen_height_, default_z_value);
}

unsigned int Screen::GetWidth() const {
    return k_screen_width_;
}

unsigned int Screen::GetHeight() const {
    return k_screen_height_;
}

void Screen::DrawPixel(unsigned int x, unsigned int y, long double depth, uint32_t color,
                       long double brightness) {
    unsigned int position = x * k_screen_height_ + y;
    if (depth < zbuffer_[position]) {
        zbuffer_[position] = depth;
        color_buffer_[position] = ApplyBrightness(color, brightness);
    }
}

uint32_t Screen::GetPixelColor(unsigned int x, unsigned int y) const {
    unsigned int position = x * k_screen_height_ + y;
    return color_buffer_[position];
}

void Screen::Clear() {
    color_buffer_.assign(k_screen_width_ * k_screen_height_, default_color_value);
    zbuffer_.assign(k_screen_width_ * k_screen_height_, default_z_value);
}

uint32_t Screen::ApplyBrightness(uint32_t color, long double brightness) {
    uint8_t r = (color >> 24) * brightness + 0.5;
    uint8_t g = ((color << 8) >> 24) * brightness + 0.5;
    uint8_t b = ((color << 16) >> 24) * brightness + 0.5;
    uint8_t a = (color << 24) >> 24;
    return (r << 24) + (g << 16) + (b << 8) + a;
}

}  // namespace rend
