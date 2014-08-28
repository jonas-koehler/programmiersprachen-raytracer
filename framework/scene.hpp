#ifndef BUW_SCENE_HPP
#define BUW_SCENE_HPP

#include "camera.hpp"
#include "composite.hpp"
#include "light.hpp"

class Scene
{
public:
  Scene();

  void camera(std::shared_ptr<Camera> const& camera);
  std::shared_ptr<Camera> const& camera() const;

  void add_shape(std::shared_ptr<Shape> const& shape);
  Composite const& root() const;

  void add_light(std::shared_ptr<Light> const& light);
  std::vector<std::shared_ptr<Light>> const& lights() const;

private:
  std::shared_ptr<Camera> camera_;
  Composite root_;
  std::vector< std::shared_ptr<Light> > lights_;
};

#endif // BUW_SCENE_HPP
