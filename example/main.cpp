#include <chrono>
#include <cmath>
#include <tuple>
#include <utility>
#include <vector>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Window/Event.hpp>

#include "Ball.hpp"
#include "CollisionTester.hpp"

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
      for (const auto& ball : _balls) {
        _window.draw(ball);
      }
      _window.display();
    }
  }

  void close() {
    _window.close();
  }

  void updatePhysic() {
    for (auto& ball : _balls) {
      CollisionDetection::IntersectionData inter;
      bool interDetected = false;

      ball.move();
      for (const auto& obj : _objects) {
        std::tie(interDetected, inter) = _collisionTester.test(ball.body(), obj);
        if (interDetected) {
          break;
        }
      }
      if (!interDetected) {
        for (const auto& obj : _balls) {
          if (&obj == &ball) {
            continue;
          }
          std::tie(interDetected, inter) = _collisionTester.test(ball.body(), obj.body());
          if (interDetected) {
            break;
          }
        }
      }

      // V’ = V – (2 * (V . N)) * N
      if (interDetected) {
        ball.move(-inter.distance);

        auto ballDirection = ball.direction();
        float dot = ballDirection.x * inter.normal.x + ballDirection.y * inter.normal.y;

        inter.normal *= (2.f * dot);
        ball.setDirection(ballDirection - inter.normal);
      }
      ball.refresh();
    }
  }

  void buildRoom() {
    _objects.emplace_back(this->makeRectangle({ 50, 50 }, 900, 10));
    _objects.emplace_back(this->makeRectangle({ 50, 50 }, 10, 900));
    _objects.emplace_back(this->makeRectangle({ 950, 950 }, -10, -900));
    _objects.emplace_back(this->makeRectangle({ 950, 950 }, -900, -10));

    _objects.emplace_back(this->makeCyclicPolygon({ 300, 300 }, 50, 8));
    _objects.emplace_back(this->makeCyclicPolygon({ 600, 800 }, 50, 4));
    _objects.emplace_back(this->makeCyclicPolygon({ 500, 500 }, 40, 6));
    _objects.emplace_back(this->makeCyclicPolygon({ 800, 300 }, 80, 3));
    _objects.emplace_back(this->makeCyclicPolygon({ 300, 700 }, 80, 3));
    _objects.emplace_back(this->makeCyclicPolygon({ 800, 600 }, 80, 30));

    _balls.emplace_back(this->makeCyclicPolygon({ 400, 150 }, 40, 20));
    _balls.emplace_back(this->makeCyclicPolygon({ 500, 250 }, 20, 20));
    _balls.emplace_back(this->makeCyclicPolygon({ 200, 250 }, 20, 20));
    _balls.emplace_back(this->makeCyclicPolygon({ 700, 250 }, 20, 20));
    _balls.emplace_back(this->makeCyclicPolygon({ 200, 250 }, 20, 20));
    _balls.emplace_back(this->makeCyclicPolygon({ 700, 250 }, 20, 20));
  }

  std::vector<sf::Vector2f> makeCyclicPolygon(const sf::Vector2f& center, float radius, size_t nbVertices) const {
    std::vector<sf::Vector2f> polygon;
    float angle = 360.f / nbVertices;

    for (size_t i = 0; i < nbVertices; ++i) {
      float angleRad = (angle * i * M_PI) / 180.f;
      polygon.emplace_back(center.x + radius * std::cos(angleRad), center.y + radius * std::sin(angleRad));
    }

    return polygon;
  }

  std::vector<sf::Vector2f> makeRectangle(const sf::Vector2f& tl, int width, int height) const {
    std::vector<sf::Vector2f> polygon;

    polygon.push_back(tl);
    polygon.emplace_back(tl.x, tl.y + height);
    polygon.emplace_back(tl.x + width, tl.y + height);
    polygon.emplace_back(tl.x + width, tl.y);

    return polygon;
  }

 private:
  std::vector<CollisionDetection::Polygon> _objects;
  std::vector<Ball> _balls;
  CollisionDetection::CollisionTester _collisionTester;

 private:
  sf::RenderWindow _window;
};

int main() {
  Window window;

  window.run();
  return 0;
}
