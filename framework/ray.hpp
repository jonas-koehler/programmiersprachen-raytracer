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
    return Ray(
      glm::vec3(t * glm::vec4(ray.o, 1.0f)),
      glm::vec3(t * glm::vec4(ray.d, 0.0f)),
      ray.depth
    );
  }
};

#endif // BUW_RAY_HPP
