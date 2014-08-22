// -----------------------------------------------------------------------------
// Copyright  : (C) 2014 Andreas-C. Bernstein
// License    : MIT (see the file LICENSE)
// Maintainer : Andreas-C. Bernstein <andreas.bernstein@uni-weimar.de>
// Stability  : experimental
//
// Renderer
// -----------------------------------------------------------------------------

#include "renderer.hpp"

Renderer::Renderer(unsigned w, unsigned h, std::string const& file, Scene const& scene, std::shared_ptr<Sampler> const& sampler)
  : width_(w)
  , height_(h)
  , colorbuffer_(w*h, Color(0.0, 0.0, 0.0))
  , filename_(file)
  , ppm_(width_, height_)
  , scene_(scene)
  , sampler_(sampler)
{}

void Renderer::render()
{
  auto cam = scene_.camera();

  while (sampler_->samples_left()) {
    auto sample = sampler_->next_sample();
    auto ray = cam.generate_ray(sample);
    Pixel px(sample.x * width_, sample.y * height_);
    px.color = shade(ray, trace(ray));
    write(px);
  }

  ppm_.save(filename_);
}

void Renderer::write(Pixel const& p)
{
  // flip pixels, because of opengl glDrawPixels
  size_t buf_pos = (width_*p.y + p.x);
  if (buf_pos >= colorbuffer_.size() || (int)buf_pos < 0) {
    std::cerr << "Fatal Error Renderer::write(Pixel p) : "
      << "pixel out of ppm_ : "
      << (int)p.x << "," << (int)p.y
      << std::endl;
  } else {
    colorbuffer_[buf_pos] = p.color;
  }

  ppm_.write(p);
}

Intersection
Renderer::trace(Ray const& ray) const
{
  auto isec = scene_.root().intersect(ray);

  // avoid too short ray intersections
  if (isec.t < RAY_EPSILON) {
    auto d = ray.d;
    auto o = ray.o + RAY_EPSILON * d;
    Ray new_ray(o, d, ray.depth);
    isec = scene_.root().intersect(ray);
  }
  return isec;
}

Color
Renderer::shade(Ray const& ray, Intersection const& isec) const
{
  if (isec.hit) {

    Color result(0.0f, 0.0f, 0.0f);
    auto p = ray.point_at(isec.t);
    auto lights = scene_.lights();
    auto material = isec.m;
    auto eye_dir = -ray.d;
    auto normal = glm::faceforward(isec.n, ray.d, isec.n);

    for (auto const& light: lights) {
      auto light_dir = glm::normalize (light.position() - p);

      Ray shadow_ray(p + RAY_EPSILON * light_dir, light_dir, ray.depth);
      if (!trace(shadow_ray).hit) {

        // diffuse light
        auto cos_phi = glm::dot (light_dir, normal);
        cos_phi = cos_phi > 0.0f ? cos_phi : 0.0f;
        result += cos_phi * light.ld() * material->kd;

        // specular highlights
        auto reflect_light_dir = glm::reflect (light_dir, normal);
        auto cos_beta = glm::dot (reflect_light_dir, eye_dir);
        auto cos_beta_m = glm::pow (cos_beta, material->m);
        result += cos_beta_m * light.ld() * material->ks;
      }

      // ambient light
      result += light.la() * material->ka;
    }

    // reflection
    Color reflected_color;

    if (material->is_reflective()) {
      auto d = glm::reflect (ray.d, normal);
      auto o = p + d * RAY_EPSILON;

      Ray reflected_ray(o, d, ray.depth-1);
      reflected_color =  material->ks * shade(reflected_ray, trace(reflected_ray));
      result += reflected_color;
    }

    // refraction
    if (material->transparency > 0.0f) {
      auto eta = material->n;

      // inside the material?
      if (glm::dot(ray.d, isec.n) < 0.0f) {
        eta = 1.0f / eta;
      }

      auto d = glm::refract (ray.d, normal, eta);
      auto o = p + d * RAY_EPSILON;

      // inner reflection
      if (d == glm::vec3(0.0f)) {
        return reflected_color;
      }

      Ray refracted_ray(o, d, ray.depth-1);
      result = material->transparency * result;
      result += (1.0f - material->transparency) * shade(refracted_ray, trace(refracted_ray));
    }

    return result;

  // background color
  } else {
    return Color(0.0f, 0.0f, 0.0f);
  }
}
