#ifndef BUW_BOX_HPP
#define BUW_BOX_HPP

#include "shape.hpp"

class Box : public Shape
{
public:
  Box(Material const& material);
  virtual Intersection const& intersect(Ray const& ray) const override;
};

#endif // BUW_BOX_HPP
