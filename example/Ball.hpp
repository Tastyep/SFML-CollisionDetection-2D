#ifndef COLLISION_DETECTION_BALL_HPP
#define COLLISION_DETECTION_BALL_HPP

#include <cmath>
#include <utility>
#include <vector>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Vector2.hpp>

#include "Polygon.hpp"

class Ball : public sf::Drawable {
 public:
  Ball(std::vector<sf::Vector2f> vertices)
    : _polygon(std::move(vertices)) {}

  const CollisionDetection::Polygon& body() const {
    return _polygon;
  };

  void draw(sf::RenderTarget& target, sf::RenderStates) const override {
    target.draw(_polygon);
  }

  void move() {
    _polygon.move(_direction * speed);
  }

  void move(float distance) {
    _polygon.move(_direction * distance);
  }

  void refresh() {
    _polygon.refresh();
  }

  void setDirection(const sf::Vector2f& direction) {
    _direction = direction;
  }
  const sf::Vector2f& direction() const {
    return _direction;
  }

 private:
  CollisionDetection::Polygon _polygon;
  sf::Vector2f _direction{ 0, 1 };
  float speed{ 10 };
};

#endif
