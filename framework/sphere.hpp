#ifndef BUW_SPHERE_HPP
#define BUW_SPHERE_HPP

#include "shape.hpp"

class Sphere : public Shape
{
public:
  Sphere(Material const& material);
  virtual Intersection const& intersect(Ray const& ray) const override;
};

#endif // BUW_SPHERE_HPP
