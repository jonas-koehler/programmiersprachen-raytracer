#ifndef BUW_SHAPE_HPP
#define BUW_SHAPE_HPP

#include "intersection.hpp"
#include "ray.hpp"
#include "bounding_box.hpp"

#include <glm/glm.hpp>

#define GLM_SWIZZLE
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

class Shape
{
public:
  Shape(std::shared_ptr<Material> const& material);

  bool intersect_bbox(Ray const& ray) const;
  virtual Intersection intersect(Ray const& ray) const = 0;

  BoundingBox bbox() const;

  virtual void translate(glm::vec3 const& t);
  virtual void scale(glm::vec3 const& s);
  virtual void rotate(float deg, glm::vec3 const& axis);

protected:
  glm::vec3 transform_normal(glm::vec3 const& n) const;

  std::shared_ptr<Material> material_;
  glm::mat4 t_;
  glm::mat4 t_inv_;
  BoundingBox bbox_;
};

#endif // BUW_SHAPE_HPP
