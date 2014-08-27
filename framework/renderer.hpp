// -----------------------------------------------------------------------------
// Copyright  : (C) 2014 Andreas-C. Bernstein
// License    : MIT (see the file LICENSE)
// Maintainer : Andreas-C. Bernstein <andreas.bernstein@uni-weimar.de>
// Stability  : experimental
//
// Renderer
// -----------------------------------------------------------------------------

#ifndef BUW_RENDERER_HPP
#define BUW_RENDERER_HPP

#include "color.hpp"
#include "pixel.hpp"
#include "ray.hpp"
#include "intersection.hpp"
#include "scene.hpp"
#include "sampler.hpp"
#include "ppmwriter.hpp"
#include "rotated_grid_sampler.hpp"
#include "standard_grid_sampler.hpp"

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <thread>

#ifndef RAY_EPSILON
  #define RAY_EPSILON 0.001f
#endif //RAY_EPSILON

#ifndef NUM_THREADS
  #define NUM_THREAD 200
#endif //NUM_THREAD

#ifndef RGSS_ANGLE
  #define RGSS_ANGLE 29.5f
#endif

class Renderer
{
public:

  enum Option {
    None             =   0x00,

    SuperSampling4x  =   0x01,
    SuperSampling16x =   0x02,

    GridSampling     =   0x04,
    RGSS             =   0x08,

    SingleThreading  =   0x10,
    MultiThreading   =   0x20,
  };

  Renderer(unsigned w, unsigned h, std::string const& file,
           Scene const& scene, unsigned char options);

  int render();

  void write(Pixel const& p);

  inline std::vector<Color> const& colorbuffer() const
  {
    return colorbuffer_;
  }

private:

  struct RenderCallback {

    RenderCallback(Renderer & r, std::shared_ptr<Sampler> const& smpl) : renderer(r), sampler(smpl) {}

    void operator() () const {
      auto cam = renderer.scene_.camera();
      while (sampler->samples_left()) {
        auto sample = sampler->next_sample();
        auto ray = cam.generate_ray(sample);
        Pixel px(std::round(sample.x * renderer.width_), std::round(sample.y * renderer.height_));
        px.color = renderer.shade(ray, renderer.trace(ray));
        renderer.write(px);
      }
    }

    Renderer & renderer;
    std::shared_ptr<Sampler> sampler;
  };

  Intersection trace(Ray const&) const;
  Color shade(Ray const& ray, Intersection const& isec) const;

  Color shadow(Ray & ray) const;

  unsigned width_;
  unsigned height_;
  std::vector<RenderCallback> render_callbacks_;
  Scene const& scene_;
  std::string filename_;
  std::vector<Color> colorbuffer_;
  std::vector<unsigned> sample_num_;
  PpmWriter ppm_;
};

#endif // #ifndef BUW_RENDERER_HPP
