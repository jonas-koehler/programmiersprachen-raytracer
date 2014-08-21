#include "shape.hpp"

Shape::Shape()
 : material_()
 , t_()
 , t_inv_()
 , bbox_(glm::vec3(-1), glm::vec3(1))
{}

Shape::Shape(std::shared_ptr<Material> const& material)
 : material_(material)
 , t_()
 , t_inv_()
 , bbox_(glm::vec3(-1), glm::vec3(1))
{}

bool
Shape::intersect_bbox(Ray const& ray) const
{
  auto object_ray = t_ * ray;
  return bbox_.intersect(object_ray);
}

BoundingBox
Shape::bbox() const
{
  return t_ * bbox_;
}

void
Shape::translate(glm::vec3 const& t)
{
  auto m = glm::translate(glm::mat4(), t);
  t_ = m * t_;
  t_inv_ = t_inv_ * m;
}

void
Shape::rotate(float rad, glm::vec3 const& axis)
{
  auto m = glm::rotate(glm::mat4(), rad, axis);
  t_ = m * t_;
  t_inv_ = t_inv_ * m;
}

void
Shape::scale(glm::vec3 const& s)
{
  auto m = glm::scale(glm::mat4(), s);
  t_ = m * t_;
  t_inv_ = t_inv_ * m;
}

glm::vec3
Shape::transform_normal(glm::vec3 const& n) const
{
  return glm::transpose(glm::mat3(t_inv_)) * n;
}
