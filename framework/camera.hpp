#ifndef BUW_CAMERA_HPP
#define BUW_CAMERA_HPP

#include "ray.hpp"
#include "sample.hpp"

#include <glm/glm.hpp>
#include <cmath>

#define GLM_SWIZZLE
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#ifndef MAX_RAY_RECURSION_DEPTH
  #define MAX_RAY_RECURSION_DEPTH 5
#endif

class Camera
{
public:
  Camera(glm::vec3 const& eye, glm::vec3 const& dir, glm::vec3 const& up, float fovx_deg);

  Ray generate_ray(Sample const& smp) const;

  void translate(glm::vec3 const& t);
  void rotate(float deg, glm::vec3 const& axis);

private:
  std::string name_;
  float focal_length_;
  glm::mat4 t_;
  glm::mat4 t_inv_;
};

#endif // BUW_CAMERA_HPP
