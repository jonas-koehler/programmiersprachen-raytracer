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
  return bbox_.intersect(t_inv_ * ray);
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
  auto m_inv = glm::translate(glm::mat4(), -t);

  t_ = m * t_;
  t_inv_ = t_inv_ * m_inv;

  t_T_ = glm::transpose(glm::mat3(t_));
  t_inv_T_ = glm::transpose(glm::mat3(t_inv_));
}

void
Shape::rotate(float rad, glm::vec3 const& axis)
{
  auto m = glm::rotate(glm::mat4(), rad, axis);
  auto m_inv = glm::rotate(glm::mat4(), -rad, axis);

  t_ = m * t_;
  t_inv_ = t_inv_ * m_inv;

  t_T_ = glm::transpose(glm::mat3(t_));
  t_inv_T_ = glm::transpose(glm::mat3(t_inv_));
}

void
Shape::scale(glm::vec3 const& s)
{
  auto m = glm::scale(glm::mat4(), s);
  auto m_inv = glm::scale(glm::mat4(), glm::vec3(1.0f / s.x, 1.0f / s.y, 1.0f / s.z));

  t_ = m * t_;
  t_inv_ = t_inv_ * m_inv;

  t_T_ = glm::transpose(glm::mat3(t_));
  t_inv_T_ = glm::transpose(glm::mat3(t_inv_));
}

glm::vec3
Shape::transform_normal(glm::vec3 const& n) const
{
  return t_T_ * n;
}
