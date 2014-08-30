#ifndef BUW_COMPOSITE_HPP
#define BUW_COMPOSITE_HPP

#include "material.hpp"
#include "shape.hpp"

#include <memory>
#include <vector>
#include <algorithm>
#include <glm/gtx/norm.hpp>

class Composite : public Shape
{
public:
  Composite();

  Intersection intersect(Ray const& ray) const override;
  void add_child(std::shared_ptr<Shape> const& child);
  void remove_child(std::shared_ptr<Shape> const& child);

  void translate(glm::vec3 const& t) override;
  void scale(glm::vec3 const& s) override;
  void rotate(float deg, glm::vec3 const& axis) override;

  std::ostream& print(std::ostream& os) const override;

  void optimize();

private:
  std::vector<std::shared_ptr<Shape>> children_;
};

#endif // BUW_COMPOSITE_HPP
