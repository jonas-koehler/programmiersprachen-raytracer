#include "camera.hpp"

Camera::Camera()
 : focal_length_(1)
 , world_transform_()
 , world_transform_inv_()
{}

Camera::Camera(glm::vec3 const& eye, glm::vec3 const& dir,
  glm::vec3 const& up, float fovx_deg)
 : focal_length_(1.0f / (2.0f * std::tan(fovx_deg / 360.0f * M_PI)))
 , world_transform_()
 , world_transform_inv_()
{
  auto u = glm::normalize(glm::cross(dir, up));
  auto v = glm::normalize(glm::cross(u, dir));
  auto n = glm::normalize(dir);

  world_transform_[0] = glm::vec4(u, 0.0f);
  world_transform_[1] = glm::vec4(v, 0.0f);
  world_transform_[2] = glm::vec4(n, 0.0f);
  world_transform_[3] = glm::vec4(eye, 1.0f);
  world_transform_inv_ = glm::inverse(world_transform_);
}

Ray
Camera::generate_ray(Sample const& smp) const
{
  glm::vec3 o(0.0f, 0.0f, 0.0f);
  glm::vec3 d(glm::vec3(smp.x - 0.5, smp.y - 0.5, -focal_length_) - o);

  Ray r = world_transform_ * Ray(o, d, MAX_RAY_RECURSION_DEPTH);
  r.d = glm::normalize(r.d);
  return r;
}

void
Camera::translate(glm::vec3 const& t)
{
  auto m = glm::translate(glm::mat4(), t);
  world_transform_ = m * world_transform_;
  world_transform_inv_ = world_transform_inv_ * m;
}

void
Camera::rotate(float rad, glm::vec3 const& axis)
{
  auto m = glm::rotate(glm::mat4(), rad, axis);
  world_transform_ = m * world_transform_;
  world_transform_inv_ = world_transform_inv_ * m;
}
