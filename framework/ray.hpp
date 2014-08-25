#ifndef BUW_RAY_HPP
#define BUW_RAY_HPP

#include <glm/glm.hpp>

#include <ostream>

struct Ray
{

  Ray(glm::vec3 const& origin, glm::vec3 const& direction, unsigned recursion_depth)
   : o(origin)
   , d(direction)
   , d_inv(1.0f / direction.x, 1.0f / direction.y, 1.0f / direction.z)
   , depth(recursion_depth)
  {};

  glm::vec3 o;
  glm::vec3 d;
  glm::vec3 d_inv;
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

  friend std::ostream& operator<<(std::ostream& os, Ray const& r)
  {
    os << "(" << r.o.x << "," << r.o.y << "," << r.o.z << ")";
    os << "(" << r.d.x << "," << r.d.y << "," << r.d.z << ")\n";
    return os;
  }

};

#endif // BUW_RAY_HPP
