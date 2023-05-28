#include "renderer.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

Renderer::Renderer() : rasterizer_(k_screen_width, k_screen_height, camera_) {
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

Matrix4 GetKeyboardChange(sf::Keyboard::Key key) {
    Matrix4 change_matrix = CreateIdentityMatrix<4>();
    switch (key) {
        case sf::Keyboard::W:
            change_matrix = CreateMoveMatrix(0, 0, -0.5);
            break;
        case sf::Keyboard::S:
            change_matrix = CreateMoveMatrix(0, 0, 0.5);
            break;
        case sf::Keyboard::A:
            change_matrix = CreateMoveMatrix(0.5, 0, 0);
            break;
        case sf::Keyboard::D:
            change_matrix = CreateMoveMatrix(-0.5, 0, 0);
            break;
        case sf::Keyboard::Left:
            change_matrix = CreateRotationMatrix(1, -3.141592 / 120);
            break;
        case sf::Keyboard::Right:
            change_matrix = CreateRotationMatrix(1, 3.141592 / 120);
            break;
        case sf::Keyboard::Up:
            change_matrix = CreateRotationMatrix(2, -3.141592 / 120);
            break;
        case sf::Keyboard::Down:
            change_matrix = CreateRotationMatrix(2, 3.141592 / 120);
            break;
        default:
            break;
    }
    return change_matrix;
}

void Renderer::Start() {
    sf::RenderWindow window(sf::VideoMode(k_screen_width, k_screen_height), "3D Renderer",
                            sf::Style::Close);
    window.setFramerateLimit(60);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                camera_.Transform(GetKeyboardChange(event.key.code));
            }
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
        unsigned int len = k_screen_width * k_screen_height;
        sf::VertexArray result_array(sf::Points, len);
        for (int i = 0; i < len; ++i) {
            result_array[i].position = sf::Vector2f(i % k_screen_width, i / k_screen_width);
            result_array[i].color = sf::Color(varray[i]);
        }
        window.draw(result_array);

        window.display();
    }
}
