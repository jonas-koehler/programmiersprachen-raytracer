#ifndef BUW_CONE_HPP
#define BUW_CONE_HPP

#include "shape.hpp"

class Cone : public Shape
{
public:
  Cone(std::shared_ptr<Material> const& material);
  Intersection intersect(Ray const& ray) const override;
  std::ostream& print(std::ostream& os) const override;
};

#endif // BUW_CONE_HPP
