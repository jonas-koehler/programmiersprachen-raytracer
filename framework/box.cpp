#include "box.hpp"

Box::Box(Material const& material)
 : Shape(material)
{}

Intersection
Box::intersect(Ray const& ray) const
{
  float tmin, tmax, tymin , tymax, tzmin, tzmax;

  if (ray.d.x >= 0.0f) {
    tmin = (-1.0f - ray.o.x) / ray.d.x;
    tmax = (1.0f - ray.o.x) / ray.d.x;
  } else {
    tmin = (1.0f - ray.o.x) / ray.d.x;
    tmax = (-1.0f - ray.o.x) / ray.d.x;
  }

  if (ray.d.y >= 0.0f) {
    tymin = (-1.0f - ray.o.y) / ray.d.y;
    tymax = (1.0f - ray.o.y) / ray.d.y;
  } else {
    tymin = (-1.0f - ray.o.y) / ray.d.y;
    tymax = (1.0f - ray.o.y) / ray.d.y;
  }

  if ( (tmin > tymax) || (tymin > tmax) )
    return Intersection(false, -1.0f, glm::vec3(), material_);

  if (tymin > tmin)
    tmin = tymin;
  if (tymax < tmax)
    tmax = tymax;

  if (ray.d.y >= 0.0f) {
    tymin = (-1.0f - ray.o.z) / ray.d.z;
    tymax = (1.0f - ray.o.z) / ray.d.z;
  } else {
    tymin = (-1.0f - ray.o.z) / ray.d.z;
    tymax = (1.0f - ray.o.z) / ray.d.z;
  }

  if ( (tmin > tzmax) || (tzmin > tmax) )
    return Intersection(false, -1.0f, glm::vec3(), material_);

  if (tzmin > tmin)
    tmin = tzmin;
  if (tzmax < tmax)
    tmax = tzmax;

  float t ;

  if (tmin < 0.0f) {
    t = tmax;
  } else {
    t = tmin;
  }

  if (t < 0.0f) {
    return Intersection(false, -1.0f, glm::vec3(), material_);
  }

  auto p = ray.point_at(t);

  glm::vec3 normal;

  if (p.x >= p.y && p.x >= p.z) {
    if (p.x >= 0.0f) {
      normal = glm::vec3(1.0f, 0.0f, 0.0f);
    } else {
      normal = glm::vec3(-1.0f, 0.0f, 0.0f);
    }
  }

  if (p.y >= p.x && p.y >= p.z) {
    if (p.y >= 0.0f) {
      normal = glm::vec3(0.0f, 1.0f, 0.0f);
    } else {
      normal = glm::vec3(0.0f, -1.0f, 0.0f);
    }
  }

  if (p.z >= p.x && p.z >= p.y) {
    if (p.z >= 0.0f) {
      normal = glm::vec3(0.0f, 0.0f, 1.0f);
    } else {
      normal = glm::vec3(0.0f, 0.0f, -1.0f);
    }
  }

  return Intersection(true, t, normal, material_);
}