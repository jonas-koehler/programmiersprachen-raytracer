#include "triangle.hpp"

Triangle::Triangle(std::shared_ptr<Material> const& material, std::array<glm::vec3, 3> const& vertices)
: Shape(material)
, vertices_(vertices)
, u_(vertices[1] - vertices[0])
, v_(vertices[2] - vertices[0])
, n_()
, denom_()
, denom_inv_()
, uu_()
, uv_()
, vv_()
{
  n_ = glm::cross(u_, v_);
  glm::normalize(n_);
  init();
}


Triangle::Triangle(std::shared_ptr<Material> const& material,
  std::array<glm::vec3, 3> const& vertices,
  std::array<glm::vec3, 3> const& normals)
: Shape(material)
, vertices_(vertices)
, u_(vertices[1] - vertices[0])
, v_(vertices[2] - vertices[0])
, n_()
, denom_()
, denom_inv_()
, uu_()
, uv_()
, vv_()
{
  n_ += normals[0];
  n_ += normals[1];
  n_ += normals[2];
  n_ /= 3.0f;

  if (glm::dot(n_, n_) == 0) {
    n_ = glm::cross(u_, v_);
  } else {
    n_ = glm::normalize(n_);
  }
  init();
}

void
Triangle::init()
{
  uv_ = glm::dot(u_,v_);
  uu_ = glm::dot(u_,u_);
  vv_ = glm::dot(v_,v_);
  denom_ = uv_*uv_ - uu_*vv_;
  denom_inv_ = 1.0f / denom_;

  bbox_.pmin = vertices_[0];
  bbox_.pmax = vertices_[2];

  // recalculate bounding box
  for (unsigned i=0; i<3; ++i) {
    for (unsigned j=0; j<3; ++j) {
      if (vertices_[i][j] < bbox_.pmin[j]) {
        bbox_.pmin[j] = vertices_[i][j];
      }
      if (vertices_[i][j] > bbox_.pmax[j]) {
        bbox_.pmax[j] = vertices_[i][j];
      }
    }
  }

  world_bbox_ = world_transform_ * bbox_;
}

Intersection
Triangle::intersect(Ray const& r) const
{
  auto ray = object_ray(r);

  auto vn = glm::dot(ray.d, n_);

  // ray and triangle plane are parallel
  if (vn == 0.0f) {
    return Intersection();
  }

  auto t = glm::dot(vertices_[0] - ray.o, n_) / vn;

  // get intersection point on the plane
  auto p = ray.point_at(t);
  auto w = p - vertices_[0];

  auto wv = glm::dot(w, v_);
  auto wu = glm::dot(w, u_);

  // calculate barycentric parameters
  auto a = (uv_ * wv - vv_ * wu) * denom_inv_;
  auto b = (uv_ * wu - uu_ * wv) * denom_inv_;

  // intersection inside triangle?
  if (a >= 0.0f && b >= 0.0f && a+b <= 1.0f) {
    return Intersection(true, t, world_normal(n_), material_);
  }

  return Intersection();
}


std::ostream&
Triangle::print(std::ostream& os) const
{
  os << "triangle";
  return os;
}
