#include "render_callback.hpp"

RenderCallback::RenderCallback(Renderer & r,
  std::shared_ptr<Sampler> const& smpl)
: renderer(r)
, sampler(smpl)
{}

void
RenderCallback::operator() () const
{
  auto cam = renderer.scene_->camera();
  while (sampler->samples_left()) {
    auto sample = sampler->next_sample();
    auto ray = cam->generate_ray(sample);

    Pixel px(std::round(sample.x * renderer.width_-1), std::round(sample.y * renderer.height_));
    px.x = std::min(renderer.width_ - 1, px.x);
    px.y = std::min(renderer.height_ - 1, px.y);
    px.color = renderer.shade(ray, renderer.trace(ray));

    renderer.write(px);
  }
  sampler->reset();
}