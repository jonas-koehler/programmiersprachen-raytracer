#include "cylinder.hpp"

Cylinder::Cylinder(std::shared_ptr<Material> const& material)
 : Shape(material)
{}

Intersection
Cylinder::intersect(Ray const& r) const
{
  auto ray = object_ray(r);

  // check for cap intersection

  float t0 = (1.0f - ray.o.z) * ray.d_inv.z;
  float t1 = (-1.0f - ray.o.z) * ray.d_inv.z;

  if (t0 > t1) { std::swap(t0, t1); }

  auto t = t0 < 0.0f ? t1 : t0;
  auto p = ray.point_at(t);

  auto rad = p.x * p.x + p.y * p.y;
  glm::vec3 n(0,0,p.z);

  if (rad < 1.0f && glm::dot(ray.d, n) < 0.0f) {
    return Intersection(true, t, world_normal(n), material_);
  }

  // check for wall intersection

  auto d_xy = glm::vec2(ray.d.x, ray.d.y);
  auto o_xy = glm::vec2(ray.o.x, ray.o.y);

  float a = glm::dot(d_xy, d_xy);
  float b = 2.0f * glm::dot(d_xy, o_xy);
  float c = glm::dot(o_xy, o_xy) - 1.0f;

  float disc = b * b - 4.0f * a * c;

  if (disc < 0.0f) { return Intersection(); }

  float discSqrt = glm::sqrt(disc);
  float a_inv = 1.0f / a;
  t0 = (-b - discSqrt)  * 0.5 * a_inv;
  t1 = (-b + discSqrt)  * 0.5 * a_inv;

  if (t0 > t1) { std::swap(t0, t1); }

  if (t1 < 0.0f) { return Intersection(); }

  t = (t0 < 0.0f) ? t1 : t0;

  p = ray.point_at(t);

  if (p.z * p.z > 1.0f) { return Intersection(); }

  n = glm::vec3(p.x, p.y, 0);

  return Intersection(true, t, world_normal(n), material_);
}

std::ostream&
Cylinder::print(std::ostream& os) const
{
  os << "a cylinder";
  return os;
}