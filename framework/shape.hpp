#ifndef BUW_SHAPE_HPP
#define BUW_SHAPE_HPP

#include "intersection.hpp"
#include "ray.hpp"
#include "bounding_box.hpp"

#include <glm/glm.hpp>

class Shape
{
public:
  Shape(Material const& material);

  bool intersect_bbox(Ray const& ray) const;
  virtual Intersection const& intersect(Ray const& ray) const = 0;

  BoundingBox bbox();

  void translate(glm::vec3 const& t);
  void scale(glm::vec3 const& s);
  void rotate(float deg, glm::vec3 const& axis);

private:
  Material material_;
  glm::mat4 t_;
  glm::mat4 t_inv_;
  BoundingBox bbox_;
};

#endif // BUW_SHAPE_HPP
