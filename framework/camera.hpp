#ifndef BUW_CAMERA_HPP
#define BUW_CAMERA_HPP

#include "ray.hpp"
#include "sample.hpp"

#include <glm/glm.hpp>

class Camera
{
public:
  Camera(glm::vec3 const& eye, glm::vec3 const& dir, glm::vec3 const& up, float fovx_deg);

  Ray generate_ray(Sample const& smp) const;

  void translate(glm::vec3 const& t);
  void scale(glm::vec3 const& s);
  void rotate(float deg, glm::vec3 const& axis);

private:
  std::string name_;
  float focal_length_;
  glm::mat4 t_;
  glm::mat4 t_inv_;
};

#endif // BUW_CAMERA_HPP
