#ifndef BUW_BOX_HPP
#define BUW_BOX_HPP

#include "shape.hpp"

class Box : public Shape
{
public:
  Box(std::shared_ptr<Material> const& material);
  Intersection intersect(Ray const& ray) const override;
  std::ostream& print(std::ostream& os) const override;
};

#endif // BUW_BOX_HPP
