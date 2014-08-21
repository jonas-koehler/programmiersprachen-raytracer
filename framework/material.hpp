#ifndef BUW_MATERIAL_HPP
#define BUW_MATERIAL_HPP

#include "color.hpp"

struct Material
{
  Material()
   : ka()
   , kd()
   , ks()
   , m(200.0f)
   , transparency(0.0f)
   , n(1.0f)
  {}

  Material(Color const& ambient, Color const& diffuse, Color const& specular,
           float specular_exponent, float opacity, float refraction_index)
   : ka(ambient)
   , kd(diffuse)
   , ks(specular)
   , m(specular_exponent)
   , transparency(1.0f - opacity)
   , n(refraction_index)
  {}

  Color ka;
  Color kd;
  Color ks;
  float m;
  float transparency;
  float n;

  bool is_reflective () {
    return ks.r > 0 && ks.g > 0 && ks.b > 0;
  }
};

#endif // BUW_MATERIAL_HPP
