// -----------------------------------------------------------------------------
// Copyright  : (C) 2014 Andreas-C. Bernstein
// License    : MIT (see the file LICENSE)
// Maintainer : Andreas-C. Bernstein <andreas.bernstein@uni-weimar.de>
// Stability  : experimental
//
// Renderer
// -----------------------------------------------------------------------------

#include "renderer.hpp"

Renderer::Renderer(unsigned w, unsigned h, std::string const& file,
                   Scene const& scene, unsigned char options)
  : width_(w)
  , height_(h)
  , colorbuffer_(w*h, Color(0.0, 0.0, 0.0))
  , sample_num_(w*h, 0.0f)
  , filename_(file)
  , ppm_(width_, height_)
  , scene_(scene)
  , render_callbacks_()
{

  unsigned samples_per_pixels_sqrt = 1;

  // super sampling active?
  if (options & Option::SuperSampling4x) {
    samples_per_pixels_sqrt = 2;
  } else if (options & Option::SuperSampling16x) {
    samples_per_pixels_sqrt = 4;
  }

  // multithreading active?
  bool multithreading = (options & Option::MultiThreading) && !(options & Option::SingleThreading);

  // RGSS or grid sampling?
  bool rgss = (options & Option::RGSS) && !(options & Option::GridSampling);


  unsigned total_threads = (multithreading) ? 200 : 1;
  float total_threads_inv = 1.0f / ((double) total_threads);

  for (unsigned i=0; i<total_threads; ++i) {

    std::shared_ptr<Sampler> sampler;

    if (rgss) {
      sampler = std::make_shared<RotatedGridSampler>(
        samples_per_pixels_sqrt * width_,
        std::ceil(samples_per_pixels_sqrt * height_ * total_threads_inv),
        RGSS_ANGLE
      );
    } else {
      sampler = std::make_shared<StandardGridSampler>(
        samples_per_pixels_sqrt * width_,
        std::ceil(samples_per_pixels_sqrt * height_ * total_threads_inv)
      );
    }

    auto ymin = i * total_threads_inv;
    auto ymax = (i + 1) * total_threads_inv;
    sampler->restrict (0, ymin, 1, ymax);

    render_callbacks_.push_back(RenderCallback(*this, sampler));
  }
}

int
Renderer::render()
{
  auto begin = std::chrono::high_resolution_clock::now();

  std::vector<std::thread> threads;
  for (auto const& cb: render_callbacks_) {
    threads.push_back(std::thread(cb));
  }
  for (auto & t: threads) {
    if (t.joinable()) {
      t.join();
    }
  }

  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();

  ppm_.save(filename_);

  return duration;
}

void
Renderer::write(Pixel const& p)
{
  // flip pixels, because of opengl glDrawPixels
  size_t buf_pos = (width_*p.y + p.x);
  if (buf_pos >= colorbuffer_.size() || (int)buf_pos < 0) {
    std::cerr << "Fatal Error Renderer::write(Pixel p) : "
      << "pixel out of ppm_ : "
      << (int)p.x << "," << (int)p.y
      << std::endl;
  } else {
    sample_num_[buf_pos] += 1;
    colorbuffer_[buf_pos] *= (sample_num_[buf_pos] - 1);
    colorbuffer_[buf_pos] += p.color;
    colorbuffer_[buf_pos] *= 1.0f  / (float) sample_num_[buf_pos];
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
    auto normal = isec.n; //glm::faceforward(isec.n, ray.d, isec.n);

    for (auto const& light: lights) {
      auto light_dir = glm::normalize (light.position() - p);

      Ray shadow_ray(p + RAY_EPSILON * light_dir, light_dir, ray.depth);
      auto shadow_color = shadow(shadow_ray);

      if (!shadow_color.is_black()) {

        // diffuse light
        auto cos_phi = glm::dot (light_dir, normal);
        cos_phi = cos_phi > 0.0f ? cos_phi : 0.0f;
        result += cos_phi * (light.ld() * material->kd);

        // specular highlights
        auto reflect_light_dir = glm::normalize (glm::reflect (light_dir, normal));
        auto cos_beta = glm::dot (reflect_light_dir, eye_dir);
        cos_beta = cos_beta > 0.0f ? cos_beta : 0.0f;
        auto cos_beta_m = glm::pow (cos_beta, material->m);
        result += cos_beta_m * (light.ld() * material->ks);
      }

      result = shadow_color * result;

      // ambient light
      result += light.la() * material->ka;
    }

    if (ray.depth > 0) {

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
        } else {
          normal *= -1;
        }

        auto d = glm::refract (ray.d, normal, eta);
        auto o = p + d * RAY_EPSILON;

        // inner reflection
        if (d == glm::vec3(0.0f)) {
          return reflected_color;
        }

        Ray refracted_ray(o, d, ray.depth-1);
        result = (1.0f - material->transparency) * result;
        result += material->transparency * shade(refracted_ray, trace(refracted_ray));
      }
    }

    return result;

  // background color
  } else {
    return Color(1.0f, 1.0f, 1.0f);
  }
}

Color
Renderer::shadow(Ray & ray) const
{
  auto isec = trace(ray);
  if (isec.hit) {
    auto material = isec.m;

    if (material->transparency > 0) {
      ray.o = ray.point_at(isec.t) + RAY_EPSILON * ray.d;

      auto normal = glm::faceforward(isec.n, ray.d, isec.n);
      auto cos_phi = glm::dot(normal, ray.d);
      //cos_phi = cos_phi >= 0.0f ? cos_phi : 0.0f;

      auto result = Color(1) - material->kd;;
      result *= material->transparency;
      result += material->kd;
      result *= cos_phi;

      if (result.is_black())
        return Color(0);
      else
        return result * shadow(ray);

    } else {
      return Color(0);
    }
  } else {
    return Color(1);
  }
}
