#ifndef BUW_SCENE_HPP
#define BUW_SCENE_HPP

#include "camera.hpp"
#include "composite.hpp"
#include "light.hpp"

class Scene
{
public:
  Scene();

  void camera(Camera const& cam);
  Camera const& camera() const;

  void add_shape(std::shared_ptr<Shape> const& shape);
  Composite const& root() const;

  void add_light(Light const& light);
  std::vector<Light> const& lights() const;

private:
  Camera cam_;
  Composite root_;
  std::vector<Light> lights_;
};

#endif // BUW_SCENE_HPP
