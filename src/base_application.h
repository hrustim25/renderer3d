#pragma once

#include "renderer.h"

#include <SFML/Graphics.hpp>

namespace rend {

class BaseApplication {
public:
    BaseApplication();
    BaseApplication(unsigned int screen_width, unsigned int screen_height);

    void Start();

private:
    void HandleSfmlEvents();
    Matrix4 GetKeyboardChange(sf::Keyboard::Key key) const;
    void UpdateScene();
    sf::VertexArray ConvertScreenToVertexArray(const Screen& screen) const;

    Renderer renderer_;

    sf::RenderWindow window_;

protected:
    Camera camera_;
    Space space_;

    Matrix4 transformation_matrix_ = CreateIdentityMatrix<4>();
};

}  // namespace rend
