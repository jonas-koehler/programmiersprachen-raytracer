#include "sampler.hpp"

Sampler::Sampler()
 : samples_left_()
{}

bool
Sampler::samples_left() const
{
  return samples_left_;
}
