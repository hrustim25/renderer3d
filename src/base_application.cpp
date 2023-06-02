#include "base_application.h"

namespace rend {

static constexpr long double k_camera_move_step = 0.5;
static constexpr long double k_camera_rotation_degree = 0.01;

BaseApplication::BaseApplication()
    : space_(),
      renderer_(),
      window_(sf::VideoMode(renderer_.GetScreen().GetWidth(), renderer_.GetScreen().GetHeight()),
              "3D Renderer", sf::Style::Close),
      camera_(renderer_.GetScreen().GetWidth(), renderer_.GetScreen().GetHeight()) {
    window_.setFramerateLimit(60);
}

BaseApplication::BaseApplication(unsigned int screen_width, unsigned int screen_height)
    : space_(),
      renderer_(screen_width, screen_height),
      window_(sf::VideoMode(renderer_.GetScreen().GetWidth(), renderer_.GetScreen().GetHeight()),
              "3D Renderer", sf::Style::Close),
      camera_(renderer_.GetScreen().GetWidth(), renderer_.GetScreen().GetHeight()) {
    window_.setFramerateLimit(60);
}

void BaseApplication::Start() {
    while (window_.isOpen()) {
        HandleSfmlEvents();
        UpdateScene();
        const Screen& screen = renderer_.DrawScene(space_, camera_);
        window_.draw(ConvertScreenToVertexArray(screen));
        window_.display();
    }
}

void BaseApplication::HandleSfmlEvents() {
    sf::Event event;
    while (window_.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window_.close();
        }
        if (event.type == sf::Event::KeyPressed) {
            camera_.Transform(GetKeyboardChange(event.key.code));
        }
    }
}

Matrix4 BaseApplication::GetKeyboardChange(sf::Keyboard::Key key) const {
    Matrix4 change_matrix = CreateIdentityMatrix<4>();
    switch (key) {
        case sf::Keyboard::W:
            // move forward
            change_matrix = CreateMoveMatrix(0, 0, -k_camera_move_step);
            break;
        case sf::Keyboard::S:
            // move backward
            change_matrix = CreateMoveMatrix(0, 0, k_camera_move_step);
            break;
        case sf::Keyboard::A:
            // move left
            change_matrix = CreateMoveMatrix(k_camera_move_step, 0, 0);
            break;
        case sf::Keyboard::D:
            // move right
            change_matrix = CreateMoveMatrix(-k_camera_move_step, 0, 0);
            break;
        case sf::Keyboard::Left:
            // left horizontal rotation
            change_matrix = CreateRotationMatrix(1, -k_camera_rotation_degree);
            break;
        case sf::Keyboard::Right:
            // right horizontal rotation
            change_matrix = CreateRotationMatrix(1, k_camera_rotation_degree);
            break;
        case sf::Keyboard::Up:
            // up vertical rotation
            change_matrix = CreateRotationMatrix(2, k_camera_rotation_degree);
            break;
        case sf::Keyboard::Down:
            // down vertical rotation
            change_matrix = CreateRotationMatrix(2, -k_camera_rotation_degree);
            break;
        default:
            break;
    }
    return change_matrix;
}

void BaseApplication::UpdateScene() {
    space_.TransformAll(transformation_matrix_);
}

sf::VertexArray BaseApplication::ConvertScreenToVertexArray(const Screen& screen) const {
    sf::VertexArray result_array(sf::Points, screen.GetWidth() * screen.GetHeight());
    for (unsigned int x = 0; x < screen.GetWidth(); ++x) {
        for (unsigned int y = 0; y < screen.GetHeight(); ++y) {
            unsigned int pos = y * screen.GetWidth() + x;
            result_array[pos].position = sf::Vector2f(x, y);
            result_array[pos].color = sf::Color(screen.GetPixelColor(x, y));
        }
    }
    return result_array;
}

}  // namespace rend
