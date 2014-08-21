#ifndef BUW_SAMPLER_HPP
#define BUW_SAMPLER_HPP

#include "sample.hpp"

class Sampler
{
public:
  Sampler();
  virtual Sample next_sample() = 0;
  bool samples_left() const;
protected:
  bool samples_left_;
};

#endif // BUW_SAMPLER_HPP
