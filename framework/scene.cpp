#include "scene.hpp"

Scene::Scene()
 : camera_()
 , root_()
 , lights_()
{
  root_ = std::make_shared<Composite>();
}

void
Scene::camera(std::shared_ptr<Camera> const& camera)
{
  camera_ = camera;
}

std::shared_ptr<Camera> const&
Scene::camera() const
{
  return camera_;
}

void
Scene::add_shape(std::shared_ptr<Shape> const& shape)
{
  root_->add_child(shape);
}

void
Scene::remove_shape(std::shared_ptr<Shape> const& shape)
{
  root_->remove_child(shape);
}


std::shared_ptr<Composite>
Scene::root() const
{
  return root_;
}

void
Scene::add_light(std::shared_ptr<Light> const& light)
{
  lights_.push_back(light);
}


std::vector< std::shared_ptr<Light> > const&
Scene::lights() const
{
  return lights_;
}