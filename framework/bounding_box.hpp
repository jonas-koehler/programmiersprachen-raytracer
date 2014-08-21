#ifndef BUW_BOUNDING_BOX_HPP
#define BUW_BOUNDING_BOX_HPP

struct BoundingBox {
  BoundingBox(glm::vec3 const& p_min, glm::vec3 const& p_max) : pmin(p_min), pmax(p_max) {}
  glm::vec3 pmin;
  glm::vec3 pmax;

  friend BoundingBox operator*(glm::mat4 const& t, BoundingBox const& bbox)
  {
    return BoundingBox(
      glm::vec3(t * glm::vec4(bbox.pmin, 1)),
      glm::vec3(t * glm::vec4(bbox.pmax, 1))
    );
  }

  bool intersect(Ray const& ray) const {
    float tmin, tmax, tymin , tymax, tzmin, tzmax;

    if (ray.d.x >= 0) {
      tmin = (pmin.x - ray.o.x) / ray.d.x;
      tmax = (pmax.x - ray.o.x) / ray.d.x;
    } else {
      tmin = (pmax.x - ray.o.x) / ray.d.x;
      tmax = (pmin.x - ray.o.x) / ray.d.x;
    }

    if (ray.d.y >= 0) {
      tymin = (pmin.y - ray.o.y) / ray.d.y;
      tymax = (pmax.y - ray.o.y) / ray.d.y;
    } else {
      tymin = (pmax.y - ray.o.y) / ray.d.y;
      tymax = (pmin.y - ray.o.y) / ray.d.y;
    }

    if ( (tmin > tymax) || (tymin > tmax) )
      return false;

    if (tymin > tmin)
      tmin = tymin;
    if (tymax < tmax)
      tmax = tymax;

    if (ray.d.y >= 0) {
      tymin = (pmin.z - ray.o.z) / ray.d.z;
      tymax = (pmax.z - ray.o.z) / ray.d.z;
    } else {
      tymin = (pmax.z - ray.o.z) / ray.d.z;
      tymax = (pmin.z - ray.o.z) / ray.d.z;
    }

    if ( (tmin > tzmax) || (tzmin > tmax) )
      return false;

    if (tzmin > tmin)
      tmin = tzmin;
    if (tzmax < tmax)
      tmax = tzmax;

    return tmax > 0;
  }
};

#endif // BUW_BOUNDING_BOX_HPP
