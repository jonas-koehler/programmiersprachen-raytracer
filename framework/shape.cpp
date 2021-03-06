#include "shape.hpp"

Shape::Shape()
 : material_()
 , world_transform_()
 , world_transform_inv_()
 , world_transform_T_()
 , world_transform_inv_T_()
 , bbox_(glm::vec3(-1), glm::vec3(1))
 , world_bbox_(glm::vec3(-1), glm::vec3(1))
 , parent_()
 , bbox_changed_(false)
{}

Shape::Shape(std::shared_ptr<Material> const& material)
 : material_(material)
 , world_transform_()
 , world_transform_inv_()
 , world_transform_T_()
 , world_transform_inv_T_()
 , bbox_(glm::vec3(-1), glm::vec3(1))
 , world_bbox_(glm::vec3(-1), glm::vec3(1))
 , parent_()
 , bbox_changed_(false)
{}

bool
Shape::intersect_bbox(Ray const& ray) const
{
  return bbox_.intersect(world_transform_inv_ * ray);
}

BoundingBox
Shape::bbox() const
{
  return world_bbox_;
}

bool
Shape::bbox_changed() const
{
  return bbox_changed_;
}

void
Shape::bbox_changed(bool b)
{
  bbox_changed_ = b;
}

void
Shape::recalculate_bbox()
{
  bbox_changed_ = true;
}

std::weak_ptr<Shape>
Shape::parent() const
{
  return parent_;
}

void
Shape::parent(std::shared_ptr<Shape> const& parent)
{
  parent_ = parent;
}

void
Shape::translate(glm::vec3 const& t)
{
  auto m = glm::translate(glm::mat4(), t);
  auto m_inv = glm::translate(glm::mat4(), -t);

  world_transform_ = m * world_transform_;
  world_transform_inv_ = world_transform_inv_ * m_inv;

  world_transform_T_ = glm::transpose(glm::mat3(world_transform_));
  world_transform_inv_T_ = glm::transpose(glm::mat3(world_transform_inv_));

  world_bbox_ = world_transform_ * bbox_;
  bbox_changed_ = true;

  if (auto p = parent_.lock()) {
    p->recalculate_bbox();
  }
}

void
Shape::rotate(float rad, glm::vec3 const& axis)
{
  auto m = glm::rotate(glm::mat4(), rad, axis);
  auto m_inv = glm::rotate(glm::mat4(), -rad, axis);

  world_transform_ = m * world_transform_;
  world_transform_inv_ = world_transform_inv_ * m_inv;

  world_transform_T_ = glm::transpose(glm::mat3(world_transform_));
  world_transform_inv_T_ = glm::transpose(glm::mat3(world_transform_inv_));

  world_bbox_ = world_transform_ * bbox_;
  bbox_changed_ = true;

  if (auto p = parent_.lock()) {
    p->recalculate_bbox();
  }
}

void
Shape::scale(glm::vec3 const& s)
{
  auto m = glm::scale(glm::mat4(), s);
  auto m_inv = glm::scale(glm::mat4(), glm::vec3(1.0f / s.x, 1.0f / s.y, 1.0f / s.z));

  world_transform_ = m * world_transform_;
  world_transform_inv_ = world_transform_inv_ * m_inv;

  world_transform_T_ = glm::transpose(glm::mat3(world_transform_));
  world_transform_inv_T_ = glm::transpose(glm::mat3(world_transform_inv_));

  world_bbox_ = world_transform_ * bbox_;
  bbox_changed_ = true;

  if (auto p = parent_.lock()) {
    p->recalculate_bbox();
  }
}

glm::vec3
Shape::world_normal(glm::vec3 const& n) const
{
  return glm::normalize(world_transform_inv_T_ * n);
}

Ray
Shape::object_ray(Ray const& r) const
{
  return world_transform_inv_ * r;
}
