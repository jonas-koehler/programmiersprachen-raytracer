#ifndef BUW_LIGHT_HPP
#define BUW_LIGHT_HPP

#include "color.hpp"

#include <glm/glm.hpp>

#define GLM_SWIZZLE
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

class Light
{
public:
  Light();
  Light(Color ambient, Color diffuse);

  void translate(glm::vec3 const& t);
  void rotate(float rad, glm::vec3 const& axis);

  glm::vec3 const& position() const;

  Color const& ld() const;
  Color const& la() const;

private:
  glm::vec3 p_;
  glm::mat4 t_;
  glm::mat4 t_inv_;
  Color la_;
  Color ld_;
};

#endif // BUW_LIGHT_HPP
