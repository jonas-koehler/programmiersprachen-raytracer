#include "composite.hpp"

Composite::Composite()
 : Shape()
 , children_()
{
  bbox_.pmin = glm::vec3(0);
  bbox_.pmax = glm::vec3(0);
}

Intersection
Composite::intersect(Ray const& r) const
{
  float min_t = 9999.0f;
  Intersection isec;

  auto ray = object_ray(r);

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
    isec.n = world_normal(isec.n);
  }

  return isec;
}

void
Composite::add_child(std::shared_ptr<Shape> const& new_child)
{
  auto child_bbox = world_transform_inv_ * new_child->bbox();
  if (children_.empty()) {
    bbox_ = child_bbox;
  } else {
    bbox_ = bbox_ + child_bbox;
  }
  auto children_size_inv = 1.0f / ((float)children_.size());
  center_of_mass_ += child_bbox.center();
  world_bbox_ = world_transform_ * bbox_;
  children_.push_back(new_child);
}

void
Composite::recalculate_bbox()
{
  bbox_changed_ = true;
  for (auto const& child: children_) {
    if (child->bbox_changed()) {
      child->bbox_changed(false);
      auto child_bbox = world_transform_inv_ * child->bbox();
      bbox_ = bbox_ + child_bbox;
      world_bbox_ = world_transform_ * bbox_;
    }
  }
  if (auto p = parent_.lock()) {
    p->recalculate_bbox();
  }
}

void
Composite::remove_child(std::shared_ptr<Shape> const& child)
{
  children_.erase(std::remove(children_.begin(), children_.end(), child));
}

void
Composite::optimize()
{
  #ifdef OCTREE_OPTIMIZATION

  if (children_.size() <= 8) { return; }

  std::vector< std::shared_ptr<Composite> > subnodes(8);
  for (unsigned i=0; i<8; ++i) {
    subnodes[i] = std::make_shared<Composite>();
  }

  {
    auto c = center_of_mass_ / (float)(children_.size());
    //auto c = bbox_.center();
    std::shared_ptr<Composite> target = subnodes[0];
    while (!children_.empty()) {
      auto current = children_.begin();
      auto child = *current;
      auto child_bbox = world_transform_ * child->bbox();
      auto cc = child_bbox.center();
      if (cc.x <= c.x && cc.y <= c.y && cc.z <= c.z) {
        target = subnodes[0];
      } else if (cc.x <= c.x && cc.y <= c.y && cc.z > c.z) {
        target = subnodes[1];
      } else if (cc.x <= c.x && cc.y > c.y && cc.z <= c.z) {
        target = subnodes[2];
      } else if (cc.x <= c.x && cc.y > c.y && cc.z > c.z) {
        target = subnodes[3];
      } else if (cc.x > c.x && cc.y <= c.y && cc.z <= c.z) {
        target = subnodes[4];
      } else if (cc.x > c.x && cc.y <= c.y && cc.z > c.z) {
        target = subnodes[5];
      } else if (cc.x > c.x && cc.y > c.y && cc.z <= c.z) {
        target = subnodes[6];
      } else if (cc.x > c.x && cc.y > c.y && cc.z > c.z) {
        target = subnodes[7];
      }
      if (target) {
        if(auto p = child->parent().lock()) {
          target->parent(p);
        }
        target->add_child(child);
        child->parent(target);
        children_.erase(children_.begin());
      }
    }
  }

  for (unsigned i=0; i<subnodes.size(); ++i) {
    auto subnode = subnodes[i];
    if (subnode->bbox().size() > 0) {
      subnode->optimize();
      children_.push_back(subnode);
    }
  }

  #endif

  #ifndef OCTREE_OPTIMIZATION

  int orig_size = children_.size();

  while (children_.size() > 2) {
    auto first = children_.begin();

    auto first_bbox = world_transform_inv_ * (*first)->bbox();

    auto second = std::next(first, 1);
    float min_v = 10e6;

    for (auto it = second; it != children_.end(); ++it) {

      auto second_bbox = world_transform_inv_ * (*it)->bbox();
      auto v = (first_bbox + second_bbox).size();

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

  #endif
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
