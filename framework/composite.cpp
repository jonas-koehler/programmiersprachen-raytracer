#include "composite.hpp"

Composite::Composite(std::shared_ptr<Material> const& material)
 : Shape(material)
 , children_()
{}

Intersection
Composite::intersect(Ray const& ray) const
{
  float min_t = 0;
  Intersection isec;

  for (auto const& child: children_) {
    if (child->bbox().intersect(ray)) {
      auto current_isec = child->intersect(ray);
      if (current_isec.t > 0 && current_isec.t < min_t) {
        isec = current_isec;
      }
    }
  }

  return isec;
}

void
Composite::add_child(std::shared_ptr<Shape> const& new_child)
{
  auto child_bbox = t_inv_ * new_child->bbox();

  if (child_bbox.pmin.x < bbox_.pmin.x ||
      child_bbox.pmin.y < bbox_.pmin.y ||
      child_bbox.pmin.z < bbox_.pmin.z) {
    bbox_.pmin = child_bbox.pmin;
  }

  if (child_bbox.pmax.x > bbox_.pmax.x ||
      child_bbox.pmax.y > bbox_.pmax.y ||
      child_bbox.pmax.z > bbox_.pmax.z) {
    bbox_.pmax = child_bbox.pmax;
  }

  children_.push_back(new_child);
}