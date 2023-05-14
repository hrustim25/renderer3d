#include "renderer.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

Renderer::Renderer() : rasterizer_(SCREEN_WIDTH_, SCREEN_HEIGHT_, camera_) {
}

Renderer::Renderer(unsigned screen_width, unsigned screen_height)
    : SCREEN_HEIGHT_(screen_height),
      SCREEN_WIDTH_(screen_width),
      rasterizer_(SCREEN_WIDTH_, SCREEN_HEIGHT_, camera_) {
}

void Renderer::SetFillMode(bool is_texture_mode) {
    is_texture_mode_ = is_texture_mode;
    rasterizer_.SetFillMode(is_texture_mode_);
}

Camera& Renderer::GetCamera() {
    return camera_;
}

Space& Renderer::GetSpace() {
    return space_;
}

void Renderer::SetRotationMatrix(const Matrix4& matrix) {
    transformation_matrix_ = matrix;
}

void Renderer::Start() {
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH_, SCREEN_HEIGHT_), "3D Renderer",
                            sf::Style::Close);
    window.setFramerateLimit(60);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();

        size_t point_count = space_.GetPointCount();
        for (size_t i = 0; i < point_count; ++i) {
            space_.TransformPoint(i, transformation_matrix_);
        }

        for (size_t i = 1; i < space_.GetNormalCount(); ++i) {
            space_.TransformNormal(i, transformation_matrix_);
        }

        space_.AddNormal(Vertex::GetLightDirection());

        rasterizer_.Clear();
        size_t polygon_count = space_.GetPolygonCount();
        for (size_t i = 0; i < polygon_count; ++i) {
            const Space::Polygon& cur_polygon = space_.GetPolygon(i);
            Vertex vs[3];
            for (size_t j = 0; j < 3; ++j) {
                if (is_texture_mode_) {
                    vs[j] = Vertex(space_.GetPoint(cur_polygon.point_indexes[j]),
                                   space_.GetTexturePointer(cur_polygon.texture_index));
                    std::pair<long double, long double> cur_tex_coords =
                        space_.GetTexCoords(cur_polygon.tex_coords_indexes[j]);
                    vs[j].SetTextureCoordinates(cur_tex_coords.first, cur_tex_coords.second);
                } else {
                    vs[j] = Vertex(space_.GetPoint(cur_polygon.point_indexes[j]),
                                   space_.GetColor(cur_polygon.colors_indexes[j]));
                }
                vs[j].SetNormal(space_.GetNormal(cur_polygon.normal_indexes[j]));
            }
            rasterizer_.DrawPolygon(vs[0], vs[1], vs[2]);
        }

        const uint32_t* varray = rasterizer_.GetPixelArray();
        unsigned len = SCREEN_WIDTH_ * SCREEN_HEIGHT_;
        sf::VertexArray result_array(sf::Points, len);
        for (int i = 0; i < len; ++i) {
            result_array[i].position = sf::Vector2f(i % SCREEN_WIDTH_, i / SCREEN_WIDTH_);
            result_array[i].color = sf::Color(varray[i]);
        }
        window.draw(result_array);

        window.display();
    }
}
