#ifndef BUW_COMPOSITE_HPP
#define BUW_COMPOSITE_HPP

#include "material.hpp"
#include "shape.hpp"

#include <memory>
#include <vector>

class Composite : public Shape
{
public:
  Composite(std::shared_ptr<Material> const& material);

  Intersection intersect(Ray const& ray) const override;
  void add_child(std::shared_ptr<Shape> const& child);

  void translate(glm::vec3 const& t) override;
  void scale(glm::vec3 const& s) override;
  void rotate(float deg, glm::vec3 const& axis) override;

private:
  std::vector<std::shared_ptr<Shape>> children_;
};

#endif // BUW_COMPOSITE_HPP
