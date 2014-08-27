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

class Renderer
{
public:

  enum class Options {

  };

  Renderer(unsigned w, unsigned h, std::string const& file,
           Scene const& scene, std::shared_ptr<Sampler> const& sampler,
           unsigned char options);

  void render();

  void write(Pixel const& p);

  inline std::vector<Color> const& colorbuffer() const
  {
    return colorbuffer_;
  }

private:
  void render_multithreaded();
  void render_singlethreaded();

  Intersection trace(Ray const&) const;
  Color shade(Ray const& ray, Intersection const& isec) const;

  Color shadow(Ray & ray) const;

  unsigned width_;
  unsigned height_;
  std::shared_ptr<Sampler> sampler_;
  Scene const& scene_;
  std::string filename_;
  std::vector<Color> colorbuffer_;
  std::vector<unsigned> sample_num_;
  PpmWriter ppm_;
  bool multithreading_;
};

#endif // #ifndef BUW_RENDERER_HPP
