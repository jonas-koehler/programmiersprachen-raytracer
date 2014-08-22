#include "light.hpp"

Light() : p_() , t_() , t_inv_() , la_() , ld_() {}

Light(Color ambient, Color diffuse)
 : p_()
 , t_()
 , t_inv_(),
 , la_(ambient)
 , ld_(diffuse)
{}

void
Light::translate(glm::vec3 const& t)
{
  auto m = glm::translate(glm::mat4(), t);
  t_ = m * t_;
  t_inv_ = t_inv_ * m;
  p = glm::vec3(t_ * glm::vec4(0.0f, 0.0f, 0.0f, 0.1f));
}

glm::vec3 const&
Light::position() const
{
  return p_;
}

Color const&
Light::ld() const
{
  return ld_;
}

Color const&
Light::la() const
{
  return la_;
}