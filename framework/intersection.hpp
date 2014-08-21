#ifndef BUW_INTERSECTION_HPP
#define BUW_INTERSECTION_HPP

#include "material.hpp"

#include <glm/glm.hpp>
#include <memory>

struct Intersection
{

  Intersection()
   : hit()
   , t(-1.0f)
   , n()
   , m()
  {}

  Intersection(bool hit_something, float ray_t, glm::vec3 const& normal, std::shared_ptr<Material> const& material)
   : hit(hit_something)
   , t(ray_t)
   , n(normal)
   , m(material)
  {}

  bool hit;
  float t;
  glm::vec3 n;
  std::shared_ptr<Material> m;
};

#endif // BUW_INTERSECTION_HPP
