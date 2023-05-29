#include <cstdio>
#include <stdexcept>

#include <jpeglib.h>
#include <png.h>

#include "image.h"

namespace rend {

uint32_t RGB::GetColors() const {
    uint32_t result = r;
    result = (result << 8) + g;
    result = (result << 8) + b;
    result = (result << 8) + 255;
    return result;
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

RGB Image::GetPixel(int x, int y) const {
    return data_[x][y];
}

void Image::SetPixel(int x, int y, const RGB& pixel) {
    data_[x][y] = pixel;
}

RGB& Image::Pixel(int x, int y) {
    x = x % Width();
    if (x < 0) {
        x += Width();
    }
    y = y % Height();
    if (y < 0) {
        y += Height();
    }
    return data_[x][y];
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
    } else if (file_type == "png") {
        ReadPng("../models/" + filename);
    } else {
        SetSize(1, 1);
    }
}

void Image::ReadJpg(const std::string& filename) {
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr err;
    FILE* infile = fopen(filename.c_str(), "rb");

    if (!infile) {
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
            SetPixel(x, y, pixel);
        }
        ++y;
    }

    (void)jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
}

void Image::ReadPng(const std::string& filename) {
    FILE* infile = fopen(filename.c_str(), "rb");

    png_bytep* row_pointers = NULL;

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(infile);
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(infile);
        return;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(infile);
        return;
    }

    png_init_io(png_ptr, infile);

    png_read_info(png_ptr, info_ptr);

    SetSize(png_get_image_width(png_ptr, info_ptr), png_get_image_height(png_ptr, info_ptr));
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    if (bit_depth == 16) {
        png_set_strip_16(png_ptr);
    }

    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(png_ptr);
    }

    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
        png_set_expand_gray_1_2_4_to_8(png_ptr);
    }

    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(png_ptr);
    }

    if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);
    }

    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        png_set_gray_to_rgb(png_ptr);
    }

    png_read_update_info(png_ptr, info_ptr);

    if (row_pointers) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(infile);
        return;
    }

    row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * Height());
    for (int y = 0; y < Height(); y++) {
        row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png_ptr, info_ptr));
    }

    png_read_image(png_ptr, row_pointers);

    for (int y = 0; y < Height(); ++y) {
        png_bytep row_ptr = row_pointers[y];
        for (int x = 0; x < Width(); ++x) {
            png_bytep pixel_ptr = &row_ptr[x * 4];
            SetPixel(x, y, RGB{.r = pixel_ptr[0], .g = pixel_ptr[1], .b = pixel_ptr[2]});
        }
    }
    free(row_pointers);

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(infile);
}

}  // namespace rend
