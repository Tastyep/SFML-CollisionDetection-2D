#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Window/Event.hpp>

class Window {
public:
  Window() : _window(sf::VideoMode(1280, 1024), "IntersectionCalculator") {
    _window.setFramerateLimit(30);
  }

  void run() {
    sf::Event event;

    while (_window.isOpen()) {
      while (_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          this->close();
        }
      }
      _window.display();
    }
  }

  void close() { _window.close(); }

private:
  sf::RenderWindow _window;
};

int main() {
  Window window;

  window.run();
  return 0;
}
