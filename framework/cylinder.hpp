#ifndef BUW_CYLINDER_HPP
#define BUW_CYLINDER_HPP

#include "shape.hpp"

class Cylinder : public Shape
{
public:
  Cylinder(Material const& material);
  virtual Intersection intersect(Ray const& ray) const override;
};

#endif // BUW_CYLINDER_HPP
