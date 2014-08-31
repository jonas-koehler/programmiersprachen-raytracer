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

class Renderer;

#include "color.hpp"
#include "pixel.hpp"
#include "ray.hpp"
#include "intersection.hpp"
#include "scene.hpp"
#include "sampler.hpp"
#include "ppmwriter.hpp"
#include "rotated_grid_sampler.hpp"
#include "standard_grid_sampler.hpp"
#include "random_sampler.hpp"
#include "render_callback.hpp"
#include "render_instruction.hpp"

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <thread>

#ifndef RAY_EPSILON
  #define RAY_EPSILON 0.01f
#endif //RAY_EPSILON

#ifndef NUM_THREADS
  #define NUM_THREAD 200
#endif //NUM_THREAD

#ifndef RGSS_ANGLE
  #define RGSS_ANGLE 29.5f
#endif

class Renderer
{
  friend class RenderCallback;

public:

  enum Option {
    None             =   0x00,

    SuperSampling4x  =   0x01,
    SuperSampling16x =   0x02,

    GridSampling     =   0x04,
    RGSS             =   0x08,
    RandomSampling   =   0x10,

    SingleThreading  =   0x20,
    MultiThreading   =   0x40,
  };

  Renderer(unsigned width, unsigned height, unsigned char options);

  int render(RenderInstruction const& ri);

  glm::ivec2 resolution() const;

  void write(Pixel const& p);

  inline std::vector<Color> const& colorbuffer() const
  {
    return colorbuffer_;
  }

private:

  enum class SamplerType {
    GridSampling,
    RGSS,
    RandomSampling
  };

  void init(RenderInstruction const& ri);

  Intersection trace(Ray const&) const;
  Color shade(Ray const& ray, Intersection const& isec) const;

  Color shadow(Ray & ray) const;

  unsigned width_;
  unsigned height_;
  std::string outfile_;
  std::shared_ptr<Scene> scene_;
  std::shared_ptr<PpmWriter> ppm_;
  unsigned samples_per_pixels_sqrt_;
  unsigned total_threads_;
  SamplerType sampler_type_;

  std::vector<RenderCallback> render_callbacks_;
  std::vector<unsigned> sample_num_;
  std::vector<Color> colorbuffer_;


};

#endif // #ifndef BUW_RENDERER_HPP
