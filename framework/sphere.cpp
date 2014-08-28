#include "sphere.hpp"

Sphere::Sphere(std::shared_ptr<Material> const& material)
 : Shape(material)
{}

Intersection
Sphere::intersect(Ray const& r) const
{
  auto ray = object_ray(r);

  float a = glm::dot(ray.d, ray.d);
  float b = 2.0f * glm::dot(ray.d, ray.o);
  float c = glm::dot(ray.o, ray.o) - 1.0;

  float disc = b * b - 4.0f * a * c;

  if (disc < 0) { return Intersection(); }

  float discSqrt = glm::sqrt(disc);
  float a_inv = 1.0f / a;
  float t0 = (-b - discSqrt)  * 0.5 * a_inv;
  float t1 = (-b + discSqrt)  * 0.5 * a_inv;

  if (t0 > t1) { std::swap(t0, t1); }

  if (t1 < 0) { return Intersection(); }

  float t = (t0 < 0.0f) ? t1 : t0;

  glm::vec3 normal = ray.point_at(t);

  return Intersection(true, t, world_normal(normal), material_);
}

std::ostream&
Sphere::print(std::ostream& os) const
{
  os << "sphere";
  return os;
}
