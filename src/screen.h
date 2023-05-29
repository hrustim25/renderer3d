#pragma once

#include <vector>

#include <inttypes.h>

namespace rend {

class Screen {
public:
    Screen();
    Screen(unsigned int screen_width, unsigned int screen_height);

    unsigned int GetWidth() const;
    unsigned int GetHeight() const;

    void DrawPixel(unsigned int x, unsigned int y, long double depth, uint32_t color,
                   long double brightness);

    uint32_t GetPixelColor(unsigned int x, unsigned int y) const;

    void Clear();

private:
    uint32_t ApplyBrightness(uint32_t color, long double brightness);

    const unsigned int k_screen_width_ = 800;
    const unsigned int k_screen_height_ = 600;
    std::vector<uint32_t> color_buffer_;
    std::vector<long double> zbuffer_;
};

}  // namespace rend
