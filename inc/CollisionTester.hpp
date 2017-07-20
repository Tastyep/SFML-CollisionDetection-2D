#ifndef COLLISION_DETECTION_COLLISION_TESTER_HPP
#define COLLISION_DETECTION_COLLISION_TESTER_HPP

#include <limits>
#include <utility>
#include <vector>

#include <SFML/System/Vector2.hpp>

#include "Polygon.hpp"

namespace CollisionDetection {

struct IntersectionData {
  sf::Vector2f normal;
  float distance{ std::numeric_limits<float>::max() };
};

class CollisionTester {
 public:
  std::pair<bool, IntersectionData> test(const std::vector<Polygon>& bodyA, const std::vector<Polygon>& bodyB) const;
  std::pair<bool, IntersectionData> test(const Polygon& polygonA, const Polygon& polygonB) const;

 private:
  std::pair<float, float> project(const std::vector<sf::Vector2f>& vertices, const sf::Vector2f& normal) const;
};

} /* namespace CollisionDetection */

#endif
