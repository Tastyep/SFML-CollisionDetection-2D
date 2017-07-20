#include "CollisionTester.hpp"

#include <algorithm>
#include <cstddef>
#include <limits>
#include <tuple>

namespace CollisionDetection {

std::pair<bool, IntersectionData> CollisionTester::test(const std::vector<Polygon>& bodyA,
                                                        const std::vector<Polygon>& bodyB) const {
  bool interResult = false;
  IntersectionData interData;

  bool cuInter;
  IntersectionData cuData;

  for (const auto& polygonA : bodyA) {
    for (const auto& polygonB : bodyB) {
      std::tie(cuInter, cuData) = this->test(polygonA, polygonB);

      if (cuInter && (!interResult || cuData.distance < interData.distance)) {
        interResult = true;
        interData = cuData;
      }
    }
  }

  return { interResult, interData };
}

std::pair<bool, IntersectionData> CollisionTester::test(const Polygon& polygonA, const Polygon& polygonB) const {
  const auto& verticesA = polygonA.vertices();
  const auto& verticesB = polygonB.vertices();
  const auto& normalsA = polygonA.normals();
  const auto& normalsB = polygonB.normals();
  IntersectionData result;
  float mtv; // minimum translation vector

  for (int pId = 0; pId < 2; ++pId) {
    const auto& normals = (pId == 0 ? normalsA : normalsB);

    for (auto normal : normals) {
      float minA, minB, maxA, maxB;

      std::tie(minA, maxA) = this->project(verticesA, normal);
      std::tie(minB, maxB) = this->project(verticesB, normal);

      if (minA > maxB || minB > maxA) { // Doesn't overlap
        return { false, {} };
      }

      mtv = std::min(maxA, maxB) - std::max(minA, minB); // The minimum overlap

      if ((minA > minB && maxA < maxB) || (minB > minA && maxB < maxA)) { // Then it is contained
        float min = std::abs(minA - minB);
        float max = std::abs(maxA - maxB);

        if (min < max) {
          mtv += min;
          normal = -normal;
        } else {
          mtv += max;
        }
      }
      if (mtv < result.distance) {
        result.normal = normal;
        result.distance = mtv;
      }
    }
  }
  return { true, result };
}

std::pair<float, float> CollisionTester::project(const std::vector<sf::Vector2f>& vertices,
                                                 const sf::Vector2f& normal) const {
  float min = std::numeric_limits<float>::max();
  float max = -min;

  for (const auto& point : vertices) {
    // To project a point on an axis use the dot product
    float dotProduct = normal.x * point.x + normal.y * point.y;

    min = std::min(min, dotProduct);
    max = std::max(max, dotProduct);
  }

  return { min, max };
}

} /* namespace CollisionDetection */
