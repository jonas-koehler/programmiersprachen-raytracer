#include "camera.hpp"

Camera::Camera(glm::vec3 const& eye, glm::vec3 const& dir, glm::vec3 const& up, float fovx_deg)
{
  auto u = glm::normalize(glm::cross(dir, up));
  auto v = glm::normalize(glm::cross(u, dir));
  auto n = glm::normalize(dir);

  t_[0] = glm::vec4(u, 0.0f);
  t_[1] = glm::vec4(v, 0.0f);
  t_[2] = glm::vec4(-n, 0.0f);
  t_[3] = glm::vec4(eye, 1.0f);
  t_inv_ = glm::inverse(cameraToWorld_);
}

Ray
Camera::generate_ray(Sample const& smp) const
{
    return Ray(
      t_ * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
      glm::normalize(t_ * )
    );
}

void
Camera::translate(glm::vec3 const& t)
{
  auto m = glm::translate(glm::mat4(), t);
  t_ = m * t_;
  t_inv_ = t_inv_ * m;
}

void
Camera::rotate(float rad, glm::vec3 const& axis)
{
  auto m = glm::rotate(glm::mat4(), rad, axis);
  t_ = m * t_;
  t_inv_ = t_inv_ * m;
}

void
Camera::scale(glm::vec3 const& s)
{
  auto m = glm::scale(glm::mat4(), s);
  t_ = m * t_;
  t_inv_ = t_inv_ * m;
}
