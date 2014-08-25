#ifndef BUW_BOUNDING_BOX_HPP
#define BUW_BOUNDING_BOX_HPP

struct BoundingBox {
  BoundingBox() : pmin(), pmax() {}
  BoundingBox(glm::vec3 const& p_min, glm::vec3 const& p_max) : pmin(p_min), pmax(p_max) {}
  glm::vec3 pmin;
  glm::vec3 pmax;

  float volume() const {
    auto d = pmax - pmin;
    return d.x * d.y * d.z;
  }

  friend BoundingBox operator+(BoundingBox const& a, BoundingBox const& b) {
    BoundingBox tmp(a);
    for (unsigned i=0; i<3; ++i) {
      tmp.pmin[i] = glm::min(a.pmin[i], b.pmin[i]);
      tmp.pmax[i] = glm::max(a.pmax[i], b.pmax[i]);
    }
    return tmp;
  }

  friend BoundingBox operator*(glm::mat4 const& t, BoundingBox const& bbox)
  {
    return BoundingBox(
      glm::vec3(t * glm::vec4(bbox.pmin, 1.0f)),
      glm::vec3(t * glm::vec4(bbox.pmax, 1.0f))
    );
  }

  friend std::ostream& operator<<(std::ostream& os, BoundingBox const& bbox)
  {
    os << "(" << bbox.pmin.x << "," << bbox.pmin.y << "," << bbox.pmin.z << ")";
    os << "(" << bbox.pmax.x << "," << bbox.pmax.y << "," << bbox.pmax.z << ")\n";
    return os;
  }

  bool intersect(Ray const& ray) const {
    float tmin, tmax, tymin , tymax, tzmin, tzmax;

    if (ray.d.x >= 0) {
      tmin = (pmin.x - ray.o.x) * ray.d_inv.x;
      tmax = (pmax.x - ray.o.x) * ray.d_inv.x;
    } else {
      tmin = (pmax.x - ray.o.x) * ray.d_inv.x;
      tmax = (pmin.x - ray.o.x) * ray.d_inv.x;
    }

    if (ray.d.y >= 0) {
      tymin = (pmin.y - ray.o.y) * ray.d_inv.y ;
      tymax = (pmax.y - ray.o.y) * ray.d_inv.y;
    } else {
      tymin = (pmax.y - ray.o.y) * ray.d_inv.y;
      tymax = (pmin.y - ray.o.y) * ray.d_inv.y;
    }

    if ( (tmin > tymax) || (tymin > tmax) )
      return false;

    if (tymin > tmin)
      tmin = tymin;
    if (tymax < tmax)
      tmax = tymax;

    if (ray.d.z >= 0) {
      tzmin = (pmin.z - ray.o.z) * ray.d_inv.z;
      tzmax = (pmax.z - ray.o.z) * ray.d_inv.z;
    } else {
      tzmin = (pmax.z - ray.o.z) * ray.d_inv.z;
      tzmax = (pmin.z - ray.o.z) * ray.d_inv.z;
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
