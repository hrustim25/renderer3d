#pragma once

#include <vector>
#include <string>

namespace rend {

struct RGB {
    int r, g, b;

    uint32_t GetColors() const;
};

class Image {
public:
    Image() = default;
    Image(size_t width, size_t height);
    Image(const std::string& filename);

    void SetSize(size_t width, size_t height);

    size_t Width() const;

    size_t Height() const;

    void SetPixel(int y, int x, const RGB& pixel);

    RGB GetPixel(int y, int x) const;

    RGB& GetPixel(int y, int x);

    void ReadImage(const std::string& filename);

private:
    void ReadJpg(const std::string& filename);

    void ReadPng(const std::string& filename);

    void ReadBmp(const std::string& filename);

    std::vector<std::vector<RGB>> data_;
};

}  // namespace rend
