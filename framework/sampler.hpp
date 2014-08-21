#ifndef BUW_SAMPLER_HPP
#define BUW_SAMPLER_HPP

#include "sample.hpp"

class Sampler
{
public:
  virtual Sample next_sample() = 0;
  bool samples_left();
private:
  bool finished_;
};

#endif // BUW_SAMPLER_HPP
