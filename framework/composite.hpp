#ifndef BUW_COMPOSITE_HPP
#define BUW_COMPOSITE_HPP

#include "material.hpp"
#include "shape.hpp"

#include <memory>
#include <vector>
#include <list>
#include <algorithm>
#include <glm/gtx/norm.hpp>

#define OCTREE_OPTIMIZATION

class Composite : public Shape
{
public:
  Composite();

  Intersection intersect(Ray const& ray) const override;
  void add_child(std::shared_ptr<Shape> const& child);
  void remove_child(std::shared_ptr<Shape> const& child);

  void recalculate_bbox() override;

  std::ostream& print(std::ostream& os) const override;

  void optimize();

private:
  std::list<std::shared_ptr<Shape>> children_;

  glm::vec3 center_of_mass_;
};

#endif // BUW_COMPOSITE_HPP
