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

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>

class Renderer
{
public:
  Renderer(unsigned w, unsigned h, std::string const& file);

  void render();
  void write(Pixel const& p);

  inline std::vector<Color> const& colorbuffer() const
  {
    return colorbuffer_;
  }

private:
  Intersection trace(Ray const&) const;
  Color shade(Ray const& ray, Intersection const& isec) const;

  unsigned width_;
  unsigned height_;
  std::shared_ptr<Sampler> sampler_;
  Scene scene_;
  std::string filename_;
  std::vector<Color> colorbuffer_;
  PpmWriter ppm_;
};

#endif // #ifndef BUW_RENDERER_HPP
