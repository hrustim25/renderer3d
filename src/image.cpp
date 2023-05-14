#include <cstdio>
#include <stdexcept>

#include <jpeglib.h>

#include "image.h"

uint32_t RGB::GetColors() const {
    uint32_t result = r;
    result = (result << 8) + g;
    result = (result << 8) + b;
    result = (result << 8) + 255;
    return result;
}

Image::Image() {
}

Image::Image(size_t width, size_t height) {
    SetSize(width, height);
}

Image::Image(const std::string& filename) {
    ReadImage(filename);
}

void Image::SetSize(size_t width, size_t height) {
    data_.assign(height, std::vector<RGB>(width));
}

size_t Image::Width() const {
    if (data_.empty()) {
        return 0;
    }
    return data_[0].size();
}

size_t Image::Height() const {
    return data_.size();
}

RGB Image::GetPixel(int y, int x) const {
    return data_[y][x];
}

void Image::SetPixel(int y, int x, const RGB& pixel) {
    data_[y][x] = pixel;
}

RGB& Image::GetPixel(int y, int x) {
    return data_[y][x];
}

void Image::ReadImage(const std::string& filename) {
    size_t last_dot_index = filename.find_last_of('.');
    if (last_dot_index == std::string::npos) {
        last_dot_index = filename.length();
    } else {
        ++last_dot_index;
    }
    std::string file_type = filename.substr(last_dot_index);
    if (file_type == "jpg" || file_type == "jpeg") {
        ReadJpg("../models/" + filename);
    } else if (file_type == "bmp") {
        ReadBmp("../models/" + filename);
    }
}

void Image::ReadJpg(const std::string& filename) {
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr err;
    FILE* infile = fopen(filename.c_str(), "rb");

    if (!infile) {
        fclose(infile);
        return;
    }

    cinfo.err = jpeg_std_error(&err);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);

    (void)jpeg_read_header(&cinfo, static_cast<boolean>(true));
    (void)jpeg_start_decompress(&cinfo);

    int row_stride = cinfo.output_width * cinfo.output_components;
    JSAMPARRAY buffer =
        (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);

    SetSize(cinfo.output_width, cinfo.output_height);

    size_t y = 0;
    while (cinfo.output_scanline < cinfo.output_height) {
        (void)jpeg_read_scanlines(&cinfo, buffer, 1);
        for (size_t x = 0; x < Width(); ++x) {
            RGB pixel;
            if (cinfo.output_components == 3) {
                pixel.r = buffer[0][x * 3];
                pixel.g = buffer[0][x * 3 + 1];
                pixel.b = buffer[0][x * 3 + 2];
            } else {
                pixel.r = pixel.g = pixel.b = buffer[0][x];
            }
            SetPixel(y, x, pixel);
        }
        ++y;
    }

    (void)jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
}

void Image::ReadBmp(const std::string& filename) {
    FILE* infile = fopen(filename.c_str(), "rb");

    if (!infile) {
        fclose(infile);
        return;
    }

    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, infile);

    SetSize(*(int*)&header[18], *(int*)&header[22]);
    int padding = (Width() * 3) % 4;
    if (padding > 0) {
        padding = 4 - padding;
    }

    size_t full_width = Width() * 3 + padding;

    unsigned char* data = new unsigned char[full_width];
    for (size_t i = 0; i < Height(); i++) {
        size_t was_read = 0;
        while (was_read < full_width) {
            was_read +=
                fread(data + was_read, sizeof(unsigned char), full_width - was_read, infile);
        }
        for (size_t j = 0; j < Width(); ++j) {
            SetPixel(i, j, {(int)data[j * 3 + 2], (int)data[j * 3 + 1], (int)data[j * 3 + 0]});
        }
    }

    delete[] data;
    fclose(infile);
}
