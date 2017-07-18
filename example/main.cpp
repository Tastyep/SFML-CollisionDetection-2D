#include <chrono>
#include <cmath>
#include <iostream>
#include <utility>
#include <vector>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Window/Event.hpp>

#include "Ball.hpp"

using namespace std::literals::chrono_literals;

class Window {
 public:
  Window()
    : _window(sf::VideoMode(1000, 1000), "IntersectionCalculator") {
    _window.setFramerateLimit(30);
  }

  void run() {
    sf::Event event;
    const int ticksPerSecond = 30;
    const int maxFrameSkip = 5;
    const auto skipTicks = 1000ms / ticksPerSecond;
    int loops;

    auto nextGameTick = std::chrono::steady_clock::now();
    this->buildRoom();
    while (_window.isOpen()) {
      while (_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          this->close();
        }
      }

      loops = 0;
      while (std::chrono::steady_clock::now() > nextGameTick && loops < maxFrameSkip) {
        this->updatePhysic();
        nextGameTick += skipTicks;
        ++loops;
      }

      // No interpolation done for the basic example.
      _window.clear(sf::Color(230, 230, 230));
      for (const auto& obj : _objects) {
        _window.draw(obj);
      }
      _window.draw(*_ball);
      _window.display();
    }
  }

  void close() {
    _window.close();
  }

  void updatePhysic() {
    _ball->update();
  }

  void buildRoom() {
    _objects.push_back(this->makeCyclicPolygon({ 500, 500 }, 490, 10));
    _objects.push_back(this->makeCyclicPolygon({ 300, 300 }, 50, 8));
    _objects.push_back(this->makeCyclicPolygon({ 600, 800 }, 50, 4));
    _objects.push_back(this->makeCyclicPolygon({ 500, 500 }, 40, 6));
    _objects.push_back(this->makeCyclicPolygon({ 800, 300 }, 80, 3));
    _objects.push_back(this->makeCyclicPolygon({ 300, 700 }, 80, 3));
    _objects.push_back(this->makeCyclicPolygon({ 800, 600 }, 80, 30));

    _ball = std::make_unique<Ball>(this->makeCyclicPolygon({ 500, 150 }, 20, 20));
  }

  sf::VertexArray makeCyclicPolygon(const sf::Vector2f& center, float radius, size_t nbVertices) const {
    sf::VertexArray polygon(sf::PrimitiveType::LineStrip, 0);
    float angle = 360.f / nbVertices;

    for (size_t i = 0; i < nbVertices + 1; ++i) {
      float angleRad = (angle * i * M_PI) / 180.f;
      polygon.append(
        sf::Vertex(sf::Vector2f(center.x + radius * std::cos(angleRad), center.y + radius * std::sin(angleRad)),
                   sf::Color::Black));
    }

    return polygon;
  }

 private:
  std::vector<sf::VertexArray> _objects;
  std::unique_ptr<Ball> _ball;

 private:
  sf::RenderWindow _window;
};

int main() {
  Window window;

  window.run();
  return 0;
}
