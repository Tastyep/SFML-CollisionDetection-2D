#ifndef POLYGON_INTERSECTION_CALCULATOR_BALL_HPP
#define POLYGON_INTERSECTION_CALCULATOR_BALL_HPP

#include <cmath>
#include <utility>
#include <vector>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>

class Ball : public sf::Drawable {
 public:
  Ball(sf::VertexArray vertices)
    : _vertices(std::move(vertices)) {}

  void draw(sf::RenderTarget& target, sf::RenderStates) const override {
    target.draw(_vertices);
  }

  void update() {}

  void setDirection(const sf::Vector2f& direction) {
    _direction = direction;
  }
  const sf::Vector2f& direction() const {
    return _direction;
  }

 private:
  std::vector<sf::Vector2i> _polygon;
  sf::VertexArray _vertices;
  sf::Vector2f _direction{ 0, 1 };
  float speed{ 10 };
};

#endif
