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

  virtual Intersection intersect(Ray const& ray) const override;
  void add_child(std::shared_ptr<Shape> const& child);

private:
  std::vector<std::shared_ptr<Shape>> children_;
};

#endif // BUW_COMPOSITE_HPP
