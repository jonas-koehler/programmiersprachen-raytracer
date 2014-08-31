#ifndef BUW_SHAPE_HPP
#define BUW_SHAPE_HPP

#include "intersection.hpp"
#include "ray.hpp"
#include "bounding_box.hpp"

#include <glm/glm.hpp>

#define GLM_SWIZZLE
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include <memory>

class Shape
{
public:
  Shape();
  Shape(std::shared_ptr<Material> const& material);

  bool intersect_bbox(Ray const& ray) const;
  virtual Intersection intersect(Ray const& ray) const = 0;

  BoundingBox bbox() const;
  bool bbox_changed() const;
  void bbox_changed(bool b);
  virtual void recalculate_bbox();

  void translate(glm::vec3 const& t);
  void scale(glm::vec3 const& s);
  void rotate(float deg, glm::vec3 const& axis);

  virtual std::ostream& print(std::ostream& os) const = 0;

  std::weak_ptr<Shape> parent() const;
  void parent(std::shared_ptr<Shape> const& parent);

  friend std::ostream& operator<<(std::ostream& os, Shape const& shape)
  {
    return shape.print(os);
  }

protected:
  glm::vec3 world_normal(glm::vec3 const& n) const;

  Ray object_ray(Ray const& r) const;

  std::shared_ptr<Material> material_;

  // object to world space and inverse
  glm::mat4 world_transform_;
  glm::mat4 world_transform_inv_;

  std::weak_ptr<Shape> parent_;

  // transposed transformations
  glm::mat3 world_transform_T_;
  glm::mat3 world_transform_inv_T_;

  BoundingBox bbox_;
  BoundingBox world_bbox_;
  bool bbox_changed_;
};

#endif // BUW_SHAPE_HPP
