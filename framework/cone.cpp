#include "cone.hpp"

Cone::Cone(std::shared_ptr<Material> const& material)
 : Shape(material)
{}

Intersection
Cone::intersect(Ray const& r) const
{
  auto ray = object_ray(r);

  // check for cap intersection

  float t = (1.0f - ray.o.z) * ray.d_inv.z;

  auto p = ray.point_at(t);

  auto rad = p.x * p.x + p.y * p.y;
  glm::vec3 n(0,0,p.z);

  if (rad < 1.0f && glm::dot(ray.d, n) < 0.0f && t > 0.0f) {
    return Intersection(true, t, world_normal(n), material_);
  }

  // check for wall intersection

  auto d_xy = glm::vec2(ray.d.x, ray.d.y);
  auto o_xy = glm::vec2(ray.o.x, ray.o.y);

  auto d2 = ray.d * ray.d;
  auto o2 = ray.o * ray.o;
  auto od = ray.o * ray.d;

  float a = d2.x + d2.y - d2.z;
  float b = 2.0f * (od.x + od.y - od.z);
  float c = o2.x + o2.y - o2.z;

  float disc = b * b - 4.0f * a * c;

  if (disc < 0.0f) { return Intersection(); }

  float disc_sqrt = glm::sqrt(disc);
  float a_inv = 1.0f / a;
  float t1 = (-b - disc_sqrt)  * 0.5 * a_inv;
  float t2 = (-b + disc_sqrt)  * 0.5 * a_inv;

  if (t1 > t2) { std::swap(t1, t2); }

  if (t2 < 0.0f) { return Intersection(); }

  t = (t1 < 0.0f) ? t2 : t1;

  p = ray.point_at(t);

  if (p.z < 0.0f || p.z > 1.0f) { return Intersection(); }

  auto pp = p * p;

  n = glm::vec3(-p.x, -p.y, p.z);



  //auto n = glm::vec3(p.x, p.y, 0);

  return Intersection(true, t, world_normal(n), material_);
}

std::ostream&
Cone::print(std::ostream& os) const
{
  os << "a cone";
  return os;
}