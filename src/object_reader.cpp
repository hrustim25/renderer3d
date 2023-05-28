#include "object_reader.h"

#include <fstream>

namespace rend {

ObjectReader::ObjectReader() {
}

Object3d ObjectReader::ReadObject(const std::string& filename, Space& space) const {
    size_t last_dot_index = filename.find_last_of('.');
    if (last_dot_index == std::string::npos) {
        last_dot_index = filename.length();
    } else {
        ++last_dot_index;
    }
    std::string file_type = filename.substr(last_dot_index);
    if (file_type == "obj") {
        return ReadObjFile(filename, space);
    } else {
        return Object3d();
    }
}

std::vector<std::string> ParseString(const std::string& str, const std::string& delim) {
    std::vector<std::string> result;
    size_t first_pos = 0;
    size_t cur_pos;
    while ((cur_pos = str.find(delim, first_pos)) != std::string::npos) {
        result.push_back(str.substr(first_pos, cur_pos - first_pos));
        if (result.back().back() == '\r') {
            result.back().pop_back();
        }
        first_pos = cur_pos + delim.length();
        if (delim == " ") {
            while (first_pos < str.size() && str[first_pos] == ' ') {
                ++first_pos;
            }
        }
    }
    if (first_pos < str.length()) {
        result.push_back(str.substr(first_pos));
        if (result.back().back() == '\r') {
            result.back().pop_back();
        }
    }
    return result;
}

Object3d ObjectReader::ReadObjFile(const std::string& filename, Space& space) const {
    std::ifstream instream(filename);
    if (!instream.is_open()) {
        return Object3d();
    }

    Object3d result;

    size_t init_point_count = space.GetPointCount();
    size_t init_tex_coords_count = space.GetTexCoordsCount();
    size_t init_normal_count = space.GetNormalCount();

    std::string data;
    while (instream) {
        std::getline(instream, data);
        std::vector<std::string> arg_array = ParseString(data, " ");
        if (arg_array.empty()) {
            continue;
        }
        if (arg_array[0] == "#") {
            continue;
        }
        if (arg_array[0] == "v") {
            long double x = std::stold(arg_array[1]);
            long double y = std::stold(arg_array[2]);
            long double z = std::stold(arg_array[3]);
            long double w = 1.0;
            space.AddPoint({x, y, z, w});
            result.AddVertexIndex(space.GetPointCount() - 1);
        } else if (arg_array[0] == "vt") {
            long double tex_x = std::stold(arg_array[1]);
            long double tex_y = std::stold(arg_array[2]);
            space.AddTextureCoords(tex_x, tex_y);
        } else if (arg_array[0] == "vn") {
            long double normal_x = std::stold(arg_array[1]);
            long double normal_y = std::stold(arg_array[2]);
            long double normal_z = std::stold(arg_array[3]);
            space.AddNormal({normal_x, normal_y, normal_z, 0});
        } else if (arg_array[0] == "f") {
            std::vector<std::string> vertex1 = ParseString(arg_array[1], "/");
            std::vector<std::string> vertex2 = ParseString(arg_array[2], "/");
            std::vector<std::string> vertex3 = ParseString(arg_array[3], "/");
            unsigned int vertex1_index = std::stoul(vertex1[0]) - 1 + init_point_count;
            unsigned int vertex2_index = std::stoul(vertex2[0]) - 1 + init_point_count;
            unsigned int vertex3_index = std::stoul(vertex3[0]) - 1 + init_point_count;
            space.AddPolygon(vertex1_index, vertex2_index, vertex3_index);
            space.SetPolygonTexture(space.GetPolygonCount() - 1, space.GetTextureCount() - 1);
            if (vertex1.size() >= 2 && vertex1[1].size() > 0) {
                unsigned int tex_coords1_index = std::stoul(vertex1[1]) - 1 + init_tex_coords_count;
                unsigned int tex_coords2_index = std::stoul(vertex2[1]) - 1 + init_tex_coords_count;
                unsigned int tex_coords3_index = std::stoul(vertex3[1]) - 1 + init_tex_coords_count;
                space.SetPolygonTextureCoords(space.GetPolygonCount() - 1, tex_coords1_index,
                                              tex_coords2_index, tex_coords3_index);
            }
            if (vertex1.size() == 3) {
                unsigned int normal1_index = std::stoul(vertex1[2]) - 1 + init_normal_count;
                unsigned int normal2_index = std::stoul(vertex2[2]) - 1 + init_normal_count;
                unsigned int normal3_index = std::stoul(vertex3[2]) - 1 + init_normal_count;
                space.SetPolygonNormals(space.GetPolygonCount() - 1, normal1_index, normal2_index,
                                        normal3_index);
            }
        } else if (arg_array[0] == "usemtl") {
            space.AddTexture(Image(arg_array[1]));
        }
    }
    return result;
}

}  // namespace rend
