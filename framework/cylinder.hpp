#ifndef BUW_CYLINDER_HPP
#define BUW_CYLINDER_HPP

#include "shape.hpp"

class Cylinder : public Shape
{
public:
  Cylinder(std::shared_ptr<Material> const& material);
  Intersection intersect(Ray const& ray) const override;
  std::ostream& print(std::ostream& os) const override;
};

#endif // BUW_CYLINDER_HPP
