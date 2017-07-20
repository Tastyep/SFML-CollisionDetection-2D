#ifndef COLLISION_DETECTION_POLYGON_HPP
#define COLLISION_DETECTION_POLYGON_HPP

#include <cmath>
#include <cstddef>
#include <utility>
#include <vector>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>

namespace CollisionDetection {

class Polygon : public sf::Drawable {
 public:
  explicit Polygon(std::vector<sf::Vector2f> vertices)
    : _vertices(std::move(vertices)) {
    size_t nbVertices = _vertices.size();

    for (size_t i = nbVertices - 1, j = 0; j < nbVertices; i = j, ++j) {
      const auto& direction = _vertices[j] - _vertices[i];
      sf::Vector2f normal{ direction.y, direction.x };

      normal /= std::sqrt(normal.x * normal.x + normal.y * normal.y);
      _normals.push_back(normal);
    }

    this->refresh();
  }

  const std::vector<sf::Vector2f>& vertices() const {
    return _vertices;
  }

  const std::vector<sf::Vector2f>& normals() const {
    return _normals;
  }

  void move(const sf::Vector2f& step) {
    for (auto& p : _vertices) {
      p += step;
    }
  }

 public:
  void draw(sf::RenderTarget& target, sf::RenderStates) const override {
    target.draw(_vertexArray);
  }

  void refresh() {
    sf::VertexArray vertices(sf::PrimitiveType::LineStrip, 0);
    size_t nbVertices = _vertices.size();

    for (size_t i = 0; i < nbVertices + 1; ++i) {
      const auto& p = _vertices[i % nbVertices];
      vertices.append(sf::Vertex(p, sf::Color::Black));
    }
    _vertexArray = std::move(vertices);
  }

 private:
  std::vector<sf::Vector2f> _vertices;
  std::vector<sf::Vector2f> _normals;

 private:
  sf::VertexArray _vertexArray;
};

} /* namespace CollisionDetection */

#endif
