#ifndef BUW_SAMPLE_HPP
#define BUW_SAMPLE_HPP

#include "color.hpp"

struct Sample {
  Sample(float image_x, float image_y) : x(image_x), y(image_y) {};

  float x;
  float y;

};

#endif // BUW_SAMPLE_HPP
