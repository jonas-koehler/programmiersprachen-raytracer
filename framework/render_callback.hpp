#ifndef BUW_RENDER_CALLBACK_HPP
#define BUW_RENDER_CALLBACK_HPP

class RenderCallback;

#include "renderer.hpp"
#include "sampler.hpp"
#include "pixel.hpp"

#include <memory>

class RenderCallback {
public:
  RenderCallback(Renderer & r, std::shared_ptr<Sampler> const& smpl);
  void operator() () const;
private:
  Renderer & renderer;
  std::shared_ptr<Sampler> sampler;
};

#endif // BUW_RENDER_CALLBACK_HPP