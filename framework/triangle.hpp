#ifndef BUW_TRIANGLE_HPP
#define BUW_TRIANGLE_HPP

#include "shape.hpp"

#include <array>

class Triangle : public Shape
{
public:
  Triangle(Material const& material, std::array<glm::vec3, 3> v);
  virtual Intersection const& intersect(Ray const& ray) const override;
private:
  std::array<glm::vec3, 3> v_;
};

#endif // BUW_TRIANGLE_HPP
