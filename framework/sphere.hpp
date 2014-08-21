#ifndef BUW_SPHERE_HPP
#define BUW_SPHERE_HPP

#include "shape.hpp"

class Sphere : public Shape
{
public:
  Sphere(std::shared_ptr<Material> const& material);
  virtual Intersection intersect(Ray const& ray) const override;
};

#endif // BUW_SPHERE_HPP
