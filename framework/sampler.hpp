#ifndef BUW_SAMPLER_HPP
#define BUW_SAMPLER_HPP

#include "sample.hpp"

class Sampler
{
public:
  Sampler();
  virtual Sample next_sample() = 0;
  virtual void reset() = 0;
  void restrict(float min_x,float min_y,float max_x,float max_y);
  unsigned total_samples() const;
  bool samples_left() const;
protected:
  bool samples_left_;
  unsigned total_samples_;
  float min_x_, min_y_, max_x_, max_y_;
};

#endif // BUW_SAMPLER_HPP
