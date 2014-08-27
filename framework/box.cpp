#include "box.hpp"

Box::Box(std::shared_ptr<Material> const& material)
 : Shape(material)
{}

Intersection
Box::intersect(Ray const& r) const
{
  float tmin, tmax, tymin , tymax, tzmin, tzmax;

  auto ray = object_ray(r);

  if (ray.d.x >= 0.0f) {
    tmin = (-1.0f - ray.o.x) * ray.d_inv.x;
    tmax = (1.0f - ray.o.x) * ray.d_inv.x;
  } else {
    tmin = (1.0f - ray.o.x) * ray.d_inv.x;
    tmax = (-1.0f - ray.o.x) * ray.d_inv.x;
  }

  if (ray.d.y >= 0.0f) {
    tymin = (-1.0f - ray.o.y) * ray.d_inv.y;
    tymax = (1.0f - ray.o.y) * ray.d_inv.y;
  } else {
    tymin = (1.0f - ray.o.y) * ray.d_inv.y;
    tymax = (-1.0f - ray.o.y) * ray.d_inv.y;
  }

  if ( (tmin > tymax) || (tymin > tmax) ) {
    return Intersection();
  }

  if (tymin > tmin)
    tmin = tymin;
  if (tymax < tmax)
    tmax = tymax;

  if (ray.d.z >= 0.0f) {
    tzmin = (-1.0f - ray.o.z) * ray.d_inv.z;
    tzmax = (1.0f - ray.o.z) * ray.d_inv.z;
  } else {
    tzmin = (1.0f - ray.o.z) * ray.d_inv.z;
    tzmax = (-1.0f - ray.o.z) * ray.d_inv.z;
  }

  if ( (tmin > tzmax) || (tzmin > tmax) ) {
    return Intersection();
  }

  if (tzmin > tmin)
    tmin = tzmin;
  if (tzmax < tmax)
    tmax = tzmax;

  float t;

  if (tmin < 0.0f) {
    t = tmax;
  } else {
    t = tmin;
  }

  if (t < 0.0f) {
    return Intersection();
  }

  auto p = ray.point_at(t);

  glm::vec3 normal;

  glm::vec3 n2 = p * p;

  if (n2.x >= n2.y && n2.x >= n2.z) {
    if (p.x >= 0)
      normal = glm::vec3(1.0f, 0.0f, 0.0f);
    else
      normal = glm::vec3(-1.0f, 0.0f, 0.0f);
  } else if (n2.y >= n2.x && n2.y >= n2.z) {
    if (p.y >= 0)
      normal = glm::vec3(0.0f, 1.0f, 0.0f);
    else
      normal = glm::vec3(0.0f, -1.0f, 0.0f);
  } else if (n2.z >= n2.x && n2.z >= n2.y) {
    if (p.z >= 0)
      normal = glm::vec3(0.0f, 0.0f, 1.0f);
    else
      normal = glm::vec3(0.0f, 0.0f, -1.0f);
  }

  return Intersection(true, t, world_normal(normal), material_);
}

std::ostream&
Box::print(std::ostream& os) const
{
  os << "box";
  return os;
}
