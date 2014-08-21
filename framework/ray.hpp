#ifndef BUW_RAY_HPP
#define BUW_RAY_HPP

#include <glm/glm.hpp>

struct Ray
{

  Ray(glm::vec3 const& origin, glm::vec3 const& direction, unsigned recursion_depth)
   : o(origin)
   , d(direction)
   , depth(recursion_depth)
  {};

  glm::vec3 o;
  glm::vec3 d;
  unsigned depth;

  glm::vec3 point_at(float t) const {
    return o + t * d;
  }

  friend Ray operator*(glm::mat4 const& t, Ray const& ray)
  {
    auto tmp (ray);
    return tmp.transform(t, false);
  }
};

#endif // BUW_RAY_HPP
