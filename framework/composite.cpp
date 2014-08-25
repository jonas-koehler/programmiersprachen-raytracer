#include "composite.hpp"

Composite::Composite()
 : Shape()
 , children_()
{}

Intersection
Composite::intersect(Ray const& r) const
{
  float min_t = 9999.0f;
  Intersection isec;

  auto ray = t_inv_ * r;

  for (auto const& child: children_) {
     if (child->intersect_bbox(ray)) {
      auto current_isec = child->intersect(ray);
      if (current_isec.hit && current_isec.t > 0 && current_isec.t < min_t) {
        isec = current_isec;
        min_t = current_isec.t;
      }
     }
  }

  if (isec.hit) {
    isec.n = transform_normal(isec.n);
  }

  return isec;
}

void
Composite::add_child(std::shared_ptr<Shape> const& new_child)
{
  auto child_bbox = t_inv_ * new_child->bbox();

  if (children_.empty()) {
    bbox_ = t_inv_ * new_child->bbox();
  } else {
    bbox_ = bbox_ + child_bbox;
  }

  children_.push_back(new_child);
}

void
Composite::optimize()
{
  int orig_size = children_.size();

  while (children_.size() > 2) {
    auto first = children_.begin();

    auto first_bbox = t_inv_ * (*first)->bbox();

    auto second = first + 1;
    float min_v = 10e6;

    for (auto it = second; it != children_.end(); ++it) {
      auto second_bbox = t_inv_ * (*second)->bbox();
      auto v = (first_bbox + second_bbox).volume();
      if (v < min_v) {
        min_v = v;
        second = it;
      }
    }

    auto first_shape = *first;
    auto second_shape = *second;

    auto composite = std::make_shared<Composite>();

    composite->add_child(first_shape);
    composite->add_child(second_shape);

    children_.push_back(composite);

    if (children_.begin() != children_.end())
      children_.erase(children_.begin());

    second = std::find(children_.begin(), children_.end(), second_shape);
    if (second != children_.end())
      children_.erase(second);
  }
}

void
Composite::translate(glm::vec3 const& t)
{
  Shape::translate(t);
  for (auto const& child: children_) {
    //child->translate(t);
  }
}

void
Composite::scale(glm::vec3 const& s)
{
  Shape::scale(s);
  for (auto const& child: children_) {
    //child->scale(s);
  }
}

void
Composite::rotate(float deg, glm::vec3 const& axis)
{
  Shape::rotate(deg, axis);
  for (auto const& child: children_) {
    //child->rotate(deg, axis);
  }
}

std::ostream&
Composite::print(std::ostream& os) const
{
  os << "(";
  for (auto const& child: children_) {
    os << *child << ",";
  }
  os << ")";
  return os;
}
