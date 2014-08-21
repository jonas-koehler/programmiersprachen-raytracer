#ifndef BUW_SAMPLE_HPP
#define BUW_SAMPLE_HPP

#include "color.hpp"

struct Sample {
  Sample(float image_x, float image_y) : x(image_x), y(image_y), cl(0,0,0) {};

  float x;
  float y;

  Color cl;
};

#endif // BUW_SAMPLE_HPP
